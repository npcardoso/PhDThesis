#ifndef __DIAGNOSIS_BENCHMARK_H__
#define __DIAGNOSIS_BENCHMARK_H__

#include "diagnosis/diagnosis_system.h"
#include "diagnosis/metrics.h"
#include "diagnosis/rank_element.h"
#include "diagnosis/randomizers/randomizer.h"
#include "diagnosis/structs/candidate.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <vector>
#include <list>
#include <set>
#include <boost/random/mersenne_twister.hpp>
#include <boost/foreach.hpp>

namespace diagnosis {
namespace metrics {
class t_benchmark_result {
public:
    inline t_benchmark_result (t_id g, t_id r, t_diagnosis_cost c) : generator(g), ranker(r), cost(c) {}
    t_id generator, ranker;
    t_diagnosis_cost cost;
};

typedef std::list<t_benchmark_result> t_benchmark_results;

template <class S>
class t_benchmark {
public:
    typedef S t_spectra_type;
    typedef typename randomizers::t_spectra_randomizer<t_spectra_type> t_randomizer_type;
    typedef std::pair<t_id, t_id> t_connection;


    const t_benchmark<S> & operator () (const t_randomizer_type & randomizer,
                                        boost::random::mt19937 & gen,
                                        t_benchmark_results & benchmark_results) const {
        t_spectra_type spectra;
        structs::t_candidate correct;


        randomizer(spectra, correct, gen);
        (* this)(spectra, correct, benchmark_results);
        return *this;
    }

    const t_benchmark<S> & operator () (const t_spectra_type & spectra,
                                        const structs::t_candidate & correct,
                                        t_benchmark_results & benchmark_results) const {
        t_candidate_generator::t_ret_type D;

        t_id last_gen_id = generators.size();


        BOOST_FOREACH(const t_connection &c, connections) {
            t_candidate_ranker::t_ret_type probs;


            if (last_gen_id != c.first) {
                D.clear();
                std::cerr << "Generating Candidates" << std::endl;
                (*generators[c.first])(spectra, D);
                std::cerr << "Done Generating Candidates" << std::endl;
                last_gen_id = c.first;
            }

            std::cerr << "Ranking Candidates" << std::endl;
            (*rankers[c.second])(spectra, D, probs);
            std::cerr << "Done Ranking Candidates" << std::endl;


            t_candidate_generator::t_ret_type::iterator it = D.begin();
            t_candidate_ranker::t_ret_type::iterator it_prob = probs.begin();

            std::list<structs::t_candidate> D_list;
            typedef diagnosis::t_rank_element<const structs::t_candidate *, t_probability_mp> t_rank_element_tmp;
            typedef std::vector<t_rank_element_tmp> t_rank;
            t_rank rank;

            while (it != D.end()) {
                D_list.push_front(*it);
                rank.push_back(t_rank_element_tmp(&(D_list.front()), *it_prob));
                it_prob++;
                it++;
            }

            sort(rank.begin(), rank.end());

            benchmark_results.push_back(t_benchmark_result(c.first, c.second, Cd_multi(correct, rank)));
        }
        return *this;
    }

    t_benchmark & add_generator (t_candidate_generator::t_ptr & generator) {
        generators.push_back(generator);
        return (*this);
    }

    t_benchmark & add_ranker (t_candidate_ranker::t_ptr & ranker) {
        rankers.push_back(ranker);
        return (*this);
    }

    t_benchmark & add_connection (t_id generator_id, t_id ranker_id) {
        connections.insert(t_connection(generator_id, ranker_id));
        return (*this);
    }

protected:
    std::set<t_connection> connections;
    std::vector<t_candidate_generator::t_ptr> generators;
    std::vector<t_candidate_ranker::t_ptr> rankers;
};
}
}
#endif