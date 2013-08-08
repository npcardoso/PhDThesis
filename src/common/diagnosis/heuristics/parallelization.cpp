#include "parallelization.h"

#include <boost/random/uniform_int_distribution.hpp>

namespace diagnosis {
namespace heuristics {
t_divide::t_divide (t_count self,
                    t_count division_count,
                    t_count stride) {
    assert(self < division_count);
    assert(division_count > 0);
    assert(stride > 0);
    this->self = self;
    this->division_count = division_count;
    this->stride = stride;
}

void t_divide::operator () (const t_spectra & spectra,
                            t_rank_element * ret,
                            const t_spectra_filter * filter) const {
    t_count component_count = spectra.get_component_count(filter);


    for (t_id i = 0; i < component_count; i++)
        if ((i / stride) % division_count != (division_count - (self + 1)))
            ret[i] = t_rank_element(ret[i].get_component(), -1);
}

std::ostream & t_divide::print (std::ostream & out) const {
    return out << "t_divide(s:" << self << ", #d:" << division_count << ", s:" << stride << ")";
}

t_random_divide::t_random_divide (t_count self,
                                  t_count division_count,
                                  unsigned int seed) {
    assert(self < division_count);
    assert(division_count > 0);
    this->self = self;
    this->division_count = division_count;
    this->gen = boost::shared_ptr<boost::mt19937> (new boost::mt19937(seed));
}

void t_random_divide::operator () (const t_spectra & spectra,
                                   t_rank_element * ret,
                                   const t_spectra_filter * filter) const {
    boost::random::uniform_int_distribution<t_component_id> distribution(0, division_count - 1);

    t_count component_count = spectra.get_component_count(filter);


    for (t_id i = 0; i < component_count; i++) {
        t_component_id owner = distribution(*gen);

        if (owner != self)
            ret[i] = t_rank_element(ret[i].get_component(), -1);
    }
}

std::ostream & t_random_divide::print (std::ostream & out) const {
    return out << "t_random_divide(s:" << self << ", #d:" << division_count << ")";
}
}
}