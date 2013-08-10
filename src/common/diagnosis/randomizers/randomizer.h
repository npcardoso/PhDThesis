#ifndef __DIAGNOSIS_SPECTRA_RANDOMIZER_RANDOMIZER_H__
#define __DIAGNOSIS_SPECTRA_RANDOMIZER_RANDOMIZER_H__

#include <boost/random/mersenne_twister.hpp>
#include "diagnosis/structs/candidate.h"
#include "utils/boost.h"

namespace diagnosis {
namespace randomizers {
template <class T>
class t_spectra_randomizer {
public:
    typedef boost::shared_ptr<t_spectra_randomizer<T> > t_ptr;
    typedef boost::shared_ptr<const t_spectra_randomizer<T> > t_const_ptr;

    virtual void operator () (T & spectra,
                              structs::t_candidate & correct_candidate,
                              boost::random::mt19937 & gen) const = 0;
};
}
}

#endif