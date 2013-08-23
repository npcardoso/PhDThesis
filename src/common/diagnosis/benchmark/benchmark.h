#ifndef __BENCHMARK_H__85256f48709bab034e918f5de145de7e4bc6af2b__
#define __BENCHMARK_H__85256f48709bab034e918f5de145de7e4bc6af2b__

#include "diagnosis/benchmark/benchmark_hook.h"
#include "diagnosis/randomizers/randomizer.h"

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
class t_benchmark {
public:
    typedef std::pair<t_id, t_id> t_connection;


    const t_benchmark & operator () (const randomizers::t_spectra_randomizer & randomizer,
                                     boost::random::mt19937 & gen,
                                     t_benchmark_hook & hook,
                                     t_count iterations) const {
        hook.init_randomizer(randomizer);

        while (iterations--) {
            structs::t_candidate correct;
            structs::t_spectra * spectra = randomizer(gen, correct);
            (* this)(* spectra, correct, hook);
            delete spectra;
        }

        return *this;
    }

    const t_benchmark & operator () (const structs::t_spectra & spectra,
                                     const structs::t_candidate & correct,
                                     t_benchmark_hook & hook) const {
        t_candidate_generator::t_ret_type D;

        t_id last_gen_id = generators.size();


        // Hook: Init
        hook.init(spectra,
                  correct);

        BOOST_FOREACH(const t_connection &c, connections) {
            t_candidate_ranker::t_ret_type probs;


            if (last_gen_id != c.first) {
                D.clear();

                // Hook: Pre-gen
                hook.pre_gen(c.first + 1);

                t_time_interval last_time = time_interval();
                (*generators[c.first])(spectra, D);

                // Hook: Post-gen
                hook.post_gen(D, time_interval() - last_time);

                last_gen_id = c.first;
            }

            // Hook: Pre-rank
            hook.pre_rank(c.second + 1);

            t_time_interval last_time = time_interval();
            (*rankers[c.second])(spectra, D, probs);

            // Hook: Post-rank
            hook.post_rank(probs, time_interval() - last_time);
        }
        // Hook: Cleanup
        hook.cleanup();
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
        assert(generator_id > 0);
        assert(ranker_id > 0);
        connections.insert(t_connection(generator_id - 1, ranker_id - 1));
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