#include "mhs.h"

#include "structs/candidate_pool.h"

#include <boost/foreach.hpp>
#include <thread>
#include <list>


namespace diagnosis {
namespace algorithms {
using namespace diagnosis::structs;
t_mhs::t_mhs (const t_const_ptr<t_similarity> similarity) {
    set_cutoff(t_ptr<t_cutoff> ());
    set_parallelization(t_ptr<t_parallelization> ());
    set_similarity(similarity);
}

void t_mhs::set_similarity (const t_const_ptr<t_similarity> similarity) {
    if (similarity.get())
        this->similarity = similarity;
    else
        this->similarity = t_const_ptr<t_similarity> (new t_ochiai());
}

void t_mhs::set_cutoff (const t_const_ptr<t_basic_cutoff> cutoff) {
    if (cutoff.get())
        this->cutoff = cutoff;
    else
        this->cutoff = t_const_ptr<t_basic_cutoff> (new t_basic_cutoff());
}

void t_mhs::set_parallelization (const t_const_ptr<t_parallelization> parallelization) {
    if (parallelization.get())
        this->parallelization = parallelization;
    else
        this->parallelization = t_const_ptr<t_parallelization> (new t_parallelization());
}

void t_mhs::operator () (const t_spectra & spectra,
                         t_trie & D,
                         const t_spectra_filter * filter) const {
    t_candidate candidate;
    t_spectra_filter tmp_filter;


    if (filter)
        tmp_filter = *filter;

    calculate(spectra, D, tmp_filter, candidate);
}

void t_mhs::calculate (const t_spectra & spectra,
                       t_trie & D,
                       t_spectra_filter & filter,
                       t_candidate & candidate,
                       t_time_interval start_time) const {
    if (spectra.is_all_pass(&filter) && candidate.size()) {
        D.add(candidate);
    }
    /* Creating complex candidates */
    else {
        if (cutoff->stop(candidate.size() + 1))
            return;

        t_count remaining_components =
            spectra.get_component_count(&filter);

        /* Ranking */
        t_ptr<t_rank> rank = (* similarity)(spectra, & filter);
        rank->sort();

        /* Remove components that will not generate new candidates */
        while (remaining_components) {
            if (rank->get_score(remaining_components - 1) < EPSILON) {
                filter.components.filter(rank->get_component(remaining_components - 1));
                remaining_components--;
            }
            else
                break;
        }

        for (t_id i = 0; i < remaining_components; i++) {
            t_component_id component = rank->get_component(i);

            /* Cutoff */
            if (cutoff->stop(*rank, i, D,
                             time_interval() - start_time))
                break;

            /* Filter */
            if (parallelization->skip(i, candidate)) {
                filter.components.filter(component);
                continue;
            }

            /* Strip component from spectra */
            t_spectra_filter strip_filter = filter;
            filter.components.filter(component);
            strip_filter.strip(component, spectra);

            /* Insert the component into the candidate */
            auto tmp = candidate.insert(component);
            assert(tmp.second);

            calculate(spectra,
                      D,
                      strip_filter,
                      candidate,
                      start_time);
            candidate.erase(tmp.first);
        }
    }
}

void t_mhs::update (const t_spectra & spectra,
                    t_trie & D,
                    const t_trie & old_D,
                    const t_spectra_filter & filter) const {
    std::list<t_candidate> candidates;


    BOOST_FOREACH(const t_candidate &candidate,
                  old_D) {
        if (spectra.is_candidate(candidate, &filter))
            D.add(candidate, false, false);
        else
            candidates.push_back(candidate);
    }
    BOOST_FOREACH(t_candidate & candidate,
                  candidates) {
        t_spectra_filter tmp_filter = filter;


        tmp_filter.strip(candidate, spectra);
        calculate(spectra, D, tmp_filter, candidate);
    }
}

void t_mhs::combine (const t_spectra & spectra,
                     t_trie & D,
                     const t_trie & D_first,
                     const t_trie & D_second,
                     const t_spectra_filter & filter_first,
                     const t_spectra_filter & filter_second) {
    std::list<t_candidate> c_first, c_second;

    {
        t_trie::iterator it = D_first.begin();

        while (it != D_first.end()) {
            if (spectra.is_candidate(*it, &filter_second))
                D.add(*it);
            else if (!D_second.is_composite(*it, true))
                c_first.push_back(*it);

            it++;
        }

        it = D_second.begin();

        while (it != D_second.end()) {
            if (spectra.is_candidate(*it, &filter_first))
                D.add(*it);
            else if (!D_first.is_composite(*it, true))
                c_second.push_back(*it);

            it++;
        }
    }
    {
        std::list<t_candidate>::iterator first_it = c_first.begin();

        while (first_it != c_first.end()) {
            std::list<t_candidate>::iterator second_it = c_second.begin();

            while (second_it != c_second.end()) {
                t_candidate tmp = *first_it;
                tmp.insert(second_it->begin(), second_it->end());
                D.add(tmp);
                second_it++;
            }

            first_it++;
        }
    }
}

t_mhs_parallel::t_mhs_parallel (const t_const_ptr<t_parallelization_factory> pf,
                                t_count n_threads) {
    // assert(n_threads > 1);
    this->pf = pf;
    this->n_threads = n_threads;
}

void t_mhs_parallel::map (t_args * args) {
    (*args->mhs)(*args->spectra,
                 args->D,
                 args->filter);
}

void t_mhs_parallel::operator () (const structs::t_spectra & spectra,
                                  structs::t_trie & D,
                                  const structs::t_spectra_filter * filter) const {
    std::list<std::thread> threads;
    std::list<t_args> args;


    for (t_id i = 0; i < n_threads; i++) {
        args.push_back(t_args());

        t_ptr<t_parallelization> p((* pf)(i, n_threads));
        t_ptr<t_mhs> mhs(new t_mhs(*this));
        mhs->set_parallelization(p);

        t_args & a = *args.rbegin();
        a.mhs = mhs;
        a.spectra = &spectra;
        a.filter = filter;

        threads.push_back(std::thread(&t_mhs_parallel::map, &a));
    }

    t_candidate_pool pool;
    BOOST_FOREACH(std::thread & t,
                  threads) {
        t.join();
    }

    while (args.size()) {
        pool.add(args.front().D);
        args.pop_front();
    }

    pool.trie(D);
}
}
}