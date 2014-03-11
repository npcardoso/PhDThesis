#include "parallelization.h"

#include <boost/random/uniform_int_distribution.hpp>

namespace diagnosis {
namespace algorithms {
bool t_parallelization::skip (t_id rank_pos,
                              t_count depth) const {
    return false;
}

t_basic_parallelization::t_basic_parallelization (t_count self,
                                                  t_count division_count,
                                                  t_count depth) {
    assert(self < division_count);
    assert(division_count > 0);
    this->self = self;
    this->division_count = division_count;
    this->depth = depth;
}

bool t_basic_parallelization::skip (t_id rank_pos,
                                    t_count depth) {
    if (check(depth))
        return skip(rank_pos);

    return false;
}

bool t_basic_parallelization::check (t_count depth) const {
    return depth == this->depth;
}

t_parallelization_random::t_parallelization_random (t_count self,
                                                    t_count division_count,
                                                    t_count depth,
                                                    unsigned int seed) : t_basic_parallelization(self,
                                                                                                 division_count,
                                                                                                 depth), gen(new boost::mt19937(seed)) {}

bool t_parallelization_random::skip (t_id rank_pos) const {
    boost::random::uniform_int_distribution<t_component_id> distribution(0, division_count - 1);
    t_component_id owner = distribution(*gen);


    return owner == self;
}

bool t_parallelization_stride::skip (t_id rank_pos) const {
    return rank_pos % division_count == (division_count - (self + 1));
}
}
}