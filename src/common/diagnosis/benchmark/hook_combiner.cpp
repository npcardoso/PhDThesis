#include "hook_combiner.h"

#include <boost/foreach.hpp>

namespace diagnosis {
namespace benchmark {
void t_hook_combiner::init_randomizer (const randomizers::t_spectra_randomizer & randomizer) {
    BOOST_FOREACH(t_benchmark_hook::t_ptr hook, hook_list) {
        hook->init_randomizer(randomizer);
    }
}

void t_hook_combiner::init (const structs::t_spectra & spectra,
                            const structs::t_candidate & correct) {
    BOOST_FOREACH(t_benchmark_hook::t_ptr hook, hook_list) {
        hook->init(spectra, correct);
    }
}

void t_hook_combiner::cleanup () {
    BOOST_FOREACH(t_benchmark_hook::t_ptr & hook, hook_list) {
        hook->cleanup();
    }
}

void t_hook_combiner::pre_gen (t_id generator_id,
                               const std::string & name) {
    BOOST_FOREACH(t_benchmark_hook::t_ptr & hook, hook_list) {
        hook->pre_gen(generator_id, name);
    }
}

void t_hook_combiner::post_gen (t_candidate_generator::t_ret_type & D,
                                t_time_interval duration) {
    BOOST_FOREACH(t_benchmark_hook::t_ptr & hook, hook_list) {
        hook->post_gen(D, duration);
    }
}

void t_hook_combiner::pre_rank (t_id ranker_id,
                                const std::string & name) {
    BOOST_FOREACH(t_benchmark_hook::t_ptr & hook, hook_list) {
        hook->pre_rank(ranker_id, name);
    }
}

void t_hook_combiner::post_rank (const t_candidate_ranker::t_ret_type & probs,
                                 t_time_interval duration) {
    BOOST_FOREACH(t_benchmark_hook::t_ptr hook, hook_list) {
        hook->post_rank(probs, duration);
    }
}

t_hook_combiner & t_hook_combiner::operator << (t_benchmark_hook * hook) {
    hook_list.push_back(t_benchmark_hook::t_ptr(hook));
    return *this;
}

t_hook_combiner & t_hook_combiner::operator << (t_benchmark_hook::t_ptr & hook) {
    hook_list.push_back(hook);
    return *this;
}
}
}