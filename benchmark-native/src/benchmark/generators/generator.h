#ifndef __GENERATOR_H_3875d8f146b1216dc5c63dcd0eae0eb3ead6b977__
#define __GENERATOR_H_3875d8f146b1216dc5c63dcd0eae0eb3ead6b977__

#include "../../structs/spectrum.h"
#include "../../utils/boost.h"
#include "../../exceptions.h"

#include <random>

namespace diagnostic {
namespace benchmark {
class t_spectrum_generator {
public:
    virtual t_spectrum * operator () (std::mt19937 & gen,
                                      t_candidate & correct_candidate) = 0;

    inline virtual std::ostream & write (std::ostream & out) const {
        throw e_not_implemented();
    }

    inline virtual ~t_spectrum_generator () {}
};

class t_generator_repeater : public t_spectrum_generator {
public:
    t_generator_repeater (t_ptr<t_spectrum_generator> generator,
                          t_count count);

    virtual t_spectrum * operator () (std::mt19937 & gen,
                                      t_candidate & correct_candidate);
    virtual std::ostream & write (std::ostream & out) const;
private:
    t_ptr<t_spectrum_generator> generator;
    t_count count;
};

class t_generator_combiner : public t_spectrum_generator, public std::list<t_ptr<t_spectrum_generator> > {
public:
    virtual t_spectrum * operator () (std::mt19937 & gen,
                                      t_candidate & correct_candidate);
};


template <class T>
t_ptr<t_spectrum_generator> generate_generators (t_count generator_count,
                                                 t_count repeat_count,
                                                 T & meta_gen,
                                                 std::mt19937 & gen) {
    t_generator_combiner * combiner = new t_generator_combiner();


    for (int i = 0; i < generator_count; i++) {
        t_ptr<t_spectrum_generator> gen_ptr(meta_gen(gen));
        t_ptr<t_spectrum_generator> rep_ptr(new t_generator_repeater(gen_ptr,
                                                                     repeat_count));
        combiner->push_back(rep_ptr);
    }

    return t_ptr<t_spectrum_generator> (combiner);
}
}
}

namespace std {
ostream & operator << (ostream & out,
                       const diagnostic::benchmark::t_spectrum_generator & system);
}
#endif