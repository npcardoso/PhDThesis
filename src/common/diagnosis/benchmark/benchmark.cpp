#include "benchmark.h"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <boost/foreach.hpp>

using namespace diagnosis::structs;
using namespace diagnosis::randomizers;

namespace diagnosis {
namespace benchmark {
const t_benchmark & t_benchmark::operator () (randomizers::t_architecture & arch,
                                              boost::random::mt19937 & gen,
                                              t_benchmark_hook & hook) const {
    while (t_system * system = arch(gen)) {
        (* this) (* system, gen, hook);
        delete system;
    }

    return *this;
}

const t_benchmark & t_benchmark::operator () (t_system & system,
                                              boost::random::mt19937 & gen,
                                              t_benchmark_hook & hook) const {
    t_collector collector(t_path_generator::t_const_ptr(new t_path_single_dir("fooodir"))); // TODO:


    // hook.init_system(collector, system);

    while (true) {
        t_candidate * correct_tmp = new t_candidate();
        t_spectra::t_const_ptr spectra(system(gen, *correct_tmp));
        t_candidate::t_const_ptr correct(correct_tmp);

        if (spectra.get() == NULL)
            break;

        (* this)(spectra, correct, hook);
    }

    return *this;
}

const t_benchmark & t_benchmark::operator () (const t_spectra::t_const_ptr & spectra,
                                              const t_candidate::t_const_ptr & correct,
                                              t_benchmark_hook & hook) const {
    assert(spectra.get() != NULL);
    assert(correct.get() != NULL);


    t_collector collector(t_path_generator::t_const_ptr(new t_path_single_dir("fooodir"))); // TODO:
    t_status_system_init sys_status(0);

    // Hook: Init
    t_status_iteration_init it_status(sys_status, 0, spectra, correct);


    hook.init(collector,
              it_status);


    t_connections::const_iterator c = connections.begin();

    while (c != connections.end()) {
        t_id last_gen_id = c->first;
        t_candidate_generator::t_ret_type * candidates_tmp(new t_candidate_generator::t_ret_type());
        ;
        t_candidate_generator::t_ret_type::t_const_ptr candidates(candidates_tmp);


        t_time_interval last_time = time_interval();
        (*generators[c->first])(*spectra, *candidates_tmp);

        // Hook: Post-gen
        t_status_post_gen gen_status(it_status,
                                     get_generator_name(c->first + 1),
                                     time_interval() - last_time,
                                     candidates);
        hook.post_gen(collector,
                      gen_status);

        // Rankers
        while (c != connections.end() && c->first == last_gen_id) {
            t_candidate_ranker::t_ret_type * probs_tmp(new t_candidate_ranker::t_ret_type());
            t_candidate_ranker::t_ret_type::t_const_ptr probs(probs_tmp);

            t_time_interval last_time = time_interval();
            (*rankers[c->second])(*spectra, *candidates, *probs_tmp);

            // Hook: Post-rank
            t_status_post_rank rank_status(gen_status,
                                           get_ranker_name(c->second + 1),
                                           time_interval() - last_time,
                                           probs);
            hook.post_rank(collector,
                           rank_status);
            c++;
        }
    }

    return *this;
}

t_benchmark & t_benchmark::add_generator (t_candidate_generator::t_ptr & generator) {
    return add_generator(generator,
                         boost::lexical_cast<std::string> (generators.size() + 1));
}

t_benchmark & t_benchmark::add_generator (t_candidate_generator::t_ptr & generator,
                                          const std::string & name) {
    assert(get_generator_id(name) == 0);

    generators.push_back(generator);
    generator_names.push_back(name);
    generator_ids[name] = generators.size();
    return (*this);
}

t_benchmark & t_benchmark::add_ranker (t_candidate_ranker::t_ptr & ranker) {
    return add_ranker(ranker,
                      boost::lexical_cast<std::string> (rankers.size() + 1));
}

t_benchmark & t_benchmark::add_ranker (t_candidate_ranker::t_ptr & ranker,
                                       const std::string & name) {
    assert(get_ranker_id(name) == 0);

    rankers.push_back(ranker);
    ranker_names.push_back(name);
    ranker_ids[name] = rankers.size();
    return (*this);
}

t_id t_benchmark::get_generator_id (const std::string & name) const {
    t_id_map::const_iterator pos = generator_ids.find(name);


    if (pos == generator_ids.end())
        return 0;

    return pos->second;
}

const std::string & t_benchmark::get_generator_name (t_id id) const {
    assert(id > 0);
    assert(id <= generator_names.size());

    return generator_names[id - 1];
}

t_id t_benchmark::get_ranker_id (const std::string & name) const {
    t_id_map::const_iterator pos = ranker_ids.find(name);


    if (pos == ranker_ids.end())
        return 0;

    return pos->second;
}

const std::string & t_benchmark::get_ranker_name (t_id id) const {
    assert(id > 0);
    assert(id <= ranker_names.size());

    return ranker_names[id - 1];
}

t_benchmark & t_benchmark::add_connection (t_id generator_id, t_id ranker_id) {
    assert(generator_id > 0);
    assert(generator_id <= generators.size());
    assert(ranker_id > 0);
    assert(ranker_id <= rankers.size());

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