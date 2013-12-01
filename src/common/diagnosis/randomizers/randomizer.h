#ifndef __RANDOMIZER_H_3875d8f146b1216dc5c63dcd0eae0eb3ead6b977__
#define __RANDOMIZER_H_3875d8f146b1216dc5c63dcd0eae0eb3ead6b977__

#include <boost/random/mersenne_twister.hpp>
#include "diagnosis/structs/spectra.h"
#include "utils/boost.h"

namespace diagnosis {
namespace randomizers {
template <class T>
class t_randomizer {
public:
    DEFINE_BOOST_SHARED_PTRS(t_randomizer<T> );

    virtual T * operator () (boost::random::mt19937 & gen) = 0;

    inline virtual std::ostream & write (std::ostream & out) const {
        throw e_not_implemented();
    }

    inline virtual ~t_randomizer () {}
};

class t_system {
public:
    DEFINE_BOOST_SHARED_PTRS(t_system);

    virtual structs::t_spectra * operator () (boost::random::mt19937 & gen,
                                              structs::t_candidate & correct_candidate) const = 0;

    virtual std::ostream & write (std::ostream & out) const = 0;

    inline virtual ~t_system () {}
};

class t_architecture {
public:
    DEFINE_BOOST_SHARED_PTRS(t_architecture);

    virtual t_system * operator () (boost::random::mt19937 & gen) const = 0;


    inline virtual ~t_architecture () {}
};
}
}

namespace std {
ostream & operator << (ostream & out, const diagnosis::randomizers::t_system & system);
}
#endif