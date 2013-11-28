#include "benchmark.h"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <boost/foreach.hpp>

using namespace diagnosis::structs;
using namespace diagnosis::randomizers;

namespace diagnosis {
namespace benchmark {
const t_benchmark & t_benchmark::operator () (randomizers::t_randomizer<randomizers::t_spectra_randomizer> & meta_randomizer,
                                              boost::random::mt19937 & gen,
                                              t_benchmark_hook & hook,
                                              t_count systems,
                                              t_count iterations) const {
    while (systems--) {
        t_spectra_randomizer * r = meta_randomizer(gen);

        if (!r)
            break;

        (* this) (* r, gen, hook, iterations);
        delete r;
    }

    return *this;
}

const t_benchmark & t_benchmark::operator () (const t_spectra_randomizer & randomizer,
                                              boost::random::mt19937 & gen,
                                              t_benchmark_hook & hook,
                                              t_count iterations) const {
    hook.init_randomizer(randomizer);

    while (iterations--) {
        t_candidate correct;
        t_spectra * spectra = randomizer(gen, correct);
        (* this)(* spectra, correct, hook);
        delete spectra;
    }

    return *this;
}

const t_benchmark & t_benchmark::operator () (const t_spectra & spectra,
                                              const t_candidate & correct,
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

t_benchmark & t_benchmark::add_generator (t_candidate_generator::t_ptr & generator) {
    return add_generator(generator,
                         boost::lexical_cast<std::string> (generators.size() + 1));
}

t_benchmark & t_benchmark::add_generator (t_candidate_generator::t_ptr & generator,
                                          const std::string & name) {
    generators.push_back(generator);
    generator_ids[name] = generators.size();
    return (*this);
}

t_benchmark & t_benchmark::add_ranker (t_candidate_ranker::t_ptr & ranker) {
    return add_ranker(ranker,
                      boost::lexical_cast<std::string> (rankers.size() + 1));
}

t_benchmark & t_benchmark::add_ranker (t_candidate_ranker::t_ptr & ranker,
                                       const std::string & name) {
    rankers.push_back(ranker);
    ranker_ids[name] = rankers.size();
    return (*this);
}

t_id t_benchmark::get_generator_id (const std::string & name) const {
    t_id_map::const_iterator pos = generator_ids.find(name);


    if (pos == generator_ids.end())
        return 0;

    return pos->second;
}

t_id t_benchmark::get_ranker_id (const std::string & name) const {
    t_id_map::const_iterator pos = ranker_ids.find(name);


    if (pos == ranker_ids.end())
        return 0;

    return pos->second;
}

t_benchmark & t_benchmark::add_connection (t_id generator_id, t_id ranker_id) {
    assert(generator_id > 0);
    assert(ranker_id > 0);
    connections.insert(t_connection(generator_id - 1, ranker_id - 1));
    return (*this);
}

t_benchmark & t_benchmark::add_connection (const std::string & generator_id,
                                           const std::string & ranker_id) {
    return add_connection(get_generator_id(generator_id),
                          get_ranker_id(ranker_id));
}
}
}