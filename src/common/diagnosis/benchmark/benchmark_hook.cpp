#include "benchmark_hook.h"

#include "utils/boost.h"

#include <fstream>

namespace diagnosis {
namespace benchmark {
t_basic_benchmark_hook::t_basic_benchmark_hook () {
    generator_id = 0;
    ranker_id = 0;
}

void t_basic_benchmark_hook::init (const structs::t_spectra & spectra,
                                   const structs::t_candidate & correct) {
    assert(get_generator_id() == 0);
    assert(get_ranker_id() == 0);
    _init(spectra, correct);
}

void t_basic_benchmark_hook::cleanup () {
    assert(get_ranker_id() == 0);
    generator_id = 0;
    _cleanup();
}

void t_basic_benchmark_hook::pre_gen (t_id generator_id) {
    assert(get_ranker_id() == 0);
    this->generator_id = generator_id;
    _pre_gen();
}

void t_basic_benchmark_hook::post_gen (t_candidate_generator::t_ret_type & D,
                                       t_time_interval duration) {
    assert(get_generator_id() != 0);
    _post_gen(D, duration);
}

void t_basic_benchmark_hook::pre_rank (t_id ranker_id) {
    assert(get_generator_id() != 0);
    assert(get_ranker_id() == 0);
    this->ranker_id = ranker_id;
    _pre_rank();
}

void t_basic_benchmark_hook::post_rank (const t_candidate_ranker::t_ret_type & probs,
                                        t_time_interval duration) {
    assert(get_generator_id() != 0);
    assert(get_ranker_id() != 0);
    _post_rank(probs, duration);
    ranker_id = 0;
}

t_id t_basic_benchmark_hook::get_generator_id () const {
    return generator_id;
}

t_id t_basic_benchmark_hook::get_ranker_id () const {
    return ranker_id;
}
}
}