#ifndef __DIAGNOSIS_BENCHMARK_H__
#define __DIAGNOSIS_BENCHMARK_H__

#include "diagnosis/diagnosis_system.h"
#include "diagnosis/benchmark/metrics.h"
#include "diagnosis/benchmark/benchmark_hooks.h"
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
namespace benchmark {
template <class S>
class t_benchmark {
public:
    typedef S t_spectra_type;
    typedef typename randomizers::t_spectra_randomizer<t_spectra_type> t_randomizer_type;
    typedef std::pair<t_id, t_id> t_connection;


    const t_benchmark<S> & operator () (const t_randomizer_type & randomizer,
                                        boost::random::mt19937 & gen,
                                        t_benchmark_hooks<t_spectra_type> & hooks) const {
        t_spectra_type spectra;
        structs::t_candidate correct;


        randomizer(spectra, correct, gen);
        (* this)(spectra, correct, hooks);

        return *this;
    }

    const t_benchmark<S> & operator () (const t_spectra_type & spectra,
                                        const structs::t_candidate & correct,
                                        t_benchmark_hooks<t_spectra_type> & hooks) const {
        t_candidate_generator::t_ret_type D;

        t_id last_gen_id = generators.size();


        // Hook: Init
        hooks.init(spectra,
                   correct);

        BOOST_FOREACH(const t_connection &c, connections) {
            t_candidate_ranker::t_ret_type probs;


            if (last_gen_id != c.first) {
                D.clear();

                // Hook: Pre-gen
                hooks.pre_gen(c.first + 1);

                (*generators[c.first])(spectra, D);

                // Hook: Post-gen
                hooks.post_gen(D);

                last_gen_id = c.first;
            }

            // Hook: Pre-rank
            hooks.pre_rank(c.second + 1);

            (*rankers[c.second])(spectra, D, probs);

            // Hook: Post-rank
            hooks.post_rank(probs);
        }
        // Hook: Cleanup
        return *this;
    }

    void batch (const t_randomizer_type & randomizer,
                boost::random::mt19937 & gen,
                std::string target_dir) const {}

    t_benchmark & add_generator (t_candidate_generator::t_ptr & generator) {
        generators.push_back(generator);
        return (*this);
    }

    t_benchmark & add_ranker (t_candidate_ranker::t_ptr & ranker) {
        rankers.push_back(ranker);
        return (*this);
    }

    t_benchmark & add_connection (t_id generator_id, t_id ranker_id) {
        assert(generator_id > 0);
        assert(ranker_id > 0);
        connections.insert(t_connection(generator_id - 1, ranker_id - 1));
        return (*this);
    }

    std::ostream & operator << (std::ostream & out) const {}

protected:
    std::set<t_connection> connections;
    std::vector<t_candidate_generator::t_ptr> generators;
    std::vector<t_candidate_ranker::t_ptr> rankers;
};
}
}
#endif