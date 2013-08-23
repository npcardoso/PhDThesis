#ifndef __RANDOMIZER_H_3875d8f146b1216dc5c63dcd0eae0eb3ead6b977__
#define __RANDOMIZER_H_3875d8f146b1216dc5c63dcd0eae0eb3ead6b977__

#include <boost/random/mersenne_twister.hpp>
#include "diagnosis/structs/spectra.h"
#include "utils/boost.h"

namespace diagnosis {
namespace randomizers {
class t_spectra_randomizer {
public:
    DEFINE_BOOST_SHARED_PTRS(t_spectra_randomizer);

    virtual structs::t_spectra * operator () (boost::random::mt19937 & gen,
                                              structs::t_candidate & correct_candidate) const = 0;
};
}
}

#endif