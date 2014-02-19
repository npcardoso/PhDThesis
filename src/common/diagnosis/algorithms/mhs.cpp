#include "mhs.h"

#include <boost/foreach.hpp>
#include <list>

namespace diagnosis {
namespace algorithms {
using namespace diagnosis::structs;
using namespace diagnosis::heuristics;

t_mhs::t_mhs (const t_heuristic & heuristic) {
    max_candidate_size = 0;
    max_candidates = 0;
    max_time = 0;
    set_heuristic(0, heuristic);
}

void t_mhs::set_heuristic (t_count start_level, const t_heuristic & heuristic) {
    heuristics[start_level] = heuristic;
}

const t_heuristic & t_mhs::get_heuristic (t_count level) const {
    t_heuristics::const_iterator it = heuristics.upper_bound(level);


    it--;
    return it->second;
}

void t_mhs::operator () (const t_spectra & spectra,
                         t_trie & D,
                         const t_spectra_filter * filter) const {
    t_candidate candidate;


    if (spectra.get_error_count(filter))
        calculate(spectra, D, filter, candidate);
}

void t_mhs::calculate (const t_spectra & spectra,
                       t_trie & D,
                       const t_spectra_filter * filter,
                       t_candidate & candidate,
                       t_time_interval start_time) const {
    t_spectra_filter tmp_filter;


    if (filter)
        tmp_filter = *filter;

    /* Candidate Length cutoff */

    if (max_candidate_size && candidate.size() + 1 > max_candidate_size)
        return;

    /* Removing singleton candidates */

    {
        t_spectra_iterator it(spectra.get_component_count(),
                              spectra.get_transaction_count(),
                              &tmp_filter);

        while (it.next_component())
            if (all_failed(it.get_component(), spectra, tmp_filter)) {
                /* Result Length cutoff */

                if (max_candidates && D.size() >= max_candidates)
                    return;

                std::pair<t_candidate::iterator, bool> tmp = candidate.insert(it.get_component());
                assert(tmp.second);

                D.add(candidate);
                candidate.erase(tmp.first);

                if (max_candidates && D.size() >= max_candidates)
                    return;

                tmp_filter.filter_component(it.get_component());
            }
    }

    /* Candidate Length cutoff */

    if (max_candidate_size && candidate.size() + 2 > max_candidate_size)
        return;

    /* Creating complex candidates */

    t_count remaining_components = spectra.get_component_count() - tmp_filter.get_filtered_component_count();

    if (remaining_components <= 0)
        return;

    /* Ranking */

    t_heuristic::t_order_buffer order_buffer = t_heuristic::get_ordering_buffer(spectra,
                                                                                &tmp_filter);

    get_heuristic(candidate.size()) (spectra, order_buffer.get(), &tmp_filter);

    for (t_id i = 0; i < remaining_components; i++) {
        heuristics::t_rank_element::t_element component = order_buffer[i].get_element();

        /* Result Length cutoff */
        if (max_candidates && D.size() >= max_candidates)
            return;

        /* Time cutoff */
        if (max_time > 0 && time_interval() - start_time > max_time)
            return;

        /* Heuristic cutoff */
        if (component == 0)
            break;

        /* Heuristic signalling skip */
        if (order_buffer[i].get_score() <= 0) {
            tmp_filter.filter_component(component);
            continue;
        }

        /* Strip component from spectra */
        t_spectra_filter strip_filter = tmp_filter;
        tmp_filter.filter_component(component);
        strip(component, spectra, strip_filter);

        /* Insert the component into the candidate */
        size_t candidate_size = candidate.size();
        std::pair<t_candidate::iterator, bool> tmp = candidate.insert(component);

        assert(tmp.second);

        calculate(spectra, D, &strip_filter, candidate, start_time);
        candidate.erase(tmp.first);

        assert(candidate_size == candidate.size());
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


        strip(candidate, spectra, tmp_filter);
        calculate(spectra, D, &tmp_filter, candidate);
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

bool t_mhs::all_failed (t_component_id component,
                        const t_spectra & spectra,
                        const t_spectra_filter & filter) const {
    t_spectra_iterator it(spectra.get_component_count(),
                          spectra.get_transaction_count(),
                          &filter);


    while (it.next_transaction()) {
        t_transaction_id transaction = it.get_transaction();

        if (!spectra.is_error(transaction))
            continue;

        bool activity = spectra.get_activations(component, transaction);

        if (!activity)
            return false;
    }

    return true;
}

void t_mhs::strip (t_candidate candidate,
                   const t_spectra & spectra,
                   t_spectra_filter & filter) const {
    t_spectra_iterator it(spectra.get_component_count(),
                          spectra.get_transaction_count(),
                          &filter);


    while (it.next_transaction()) {
        t_candidate::const_iterator c_it = candidate.begin();

        while (c_it != candidate.end()) {
            t_transaction_id transaction = it.get_transaction();

            if (spectra.get_activations(*(c_it++), transaction)) {
                filter.filter_transaction(transaction);
                break;
            }
        }
    }

    t_candidate::const_iterator c_it = candidate.begin();

    while (c_it != candidate.end())
        filter.filter_component(*(c_it++));
}

void t_mhs::strip (t_component_id component,
                   const t_spectra & spectra,
                   t_spectra_filter & filter) const {
    t_spectra_iterator it(spectra.get_component_count(),
                          spectra.get_transaction_count(),
                          &filter);


    while (it.next_transaction()) {
        t_transaction_id transaction = it.get_transaction();
        bool activity = spectra.is_active(component, transaction);

        if (activity)
            filter.filter_transaction(transaction);
    }

    filter.filter_component(component);
}
}
}