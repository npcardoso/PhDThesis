#ifndef __RANDOMIZER_H_3875d8f146b1216dc5c63dcd0eae0eb3ead6b977__
#define __RANDOMIZER_H_3875d8f146b1216dc5c63dcd0eae0eb3ead6b977__

#include <boost/random/mersenne_twister.hpp>
#include "diagnosis/structs/candidate.h"
#include "utils/boost.h"

namespace diagnosis {
namespace randomizers {
template <class T>
class t_spectra_randomizer {
public:
    typedef t_spectra_randomizer<T> t_self_type;
    typedef boost::shared_ptr<t_spectra_randomizer<T> > t_ptr;
    typedef boost::shared_ptr<const t_spectra_randomizer<T> > t_const_ptr;

    virtual const t_self_type & operator () (T & spectra,
                                             structs::t_candidate & correct_candidate,
                                             boost::random::mt19937 & gen) const = 0;
};
}
}

#endif