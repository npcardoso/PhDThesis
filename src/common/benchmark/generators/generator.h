#ifndef __GENERATOR_H_3875d8f146b1216dc5c63dcd0eae0eb3ead6b977__
#define __GENERATOR_H_3875d8f146b1216dc5c63dcd0eae0eb3ead6b977__

#include "structs/spectra.h"
#include "utils/boost.h"
#include "exceptions.h"

namespace diagnosis {
namespace benchmark {
class t_spectra_generator {
public:
    virtual structs::t_spectra * operator () (std::mt19937 & gen,
                                              structs::t_candidate & correct_candidate) = 0;

    inline virtual std::ostream & write (std::ostream & out) const {
        throw e_not_implemented();
    }

    inline virtual ~t_spectra_generator () {}
};

class t_generator_repeater : public t_spectra_generator {
public:
    t_generator_repeater (t_ptr<t_spectra_generator> generator,
                          t_count count);

    virtual structs::t_spectra * operator () (std::mt19937 & gen,
                                              structs::t_candidate & correct_candidate);
    virtual std::ostream & write (std::ostream & out) const;
private:
    t_ptr<t_spectra_generator> generator;
    t_count count;
};

class t_generator_combiner : public t_spectra_generator, public std::list<t_ptr<t_spectra_generator> > {
public:
    virtual structs::t_spectra * operator () (std::mt19937 & gen,
                                              structs::t_candidate & correct_candidate);
};


template <class T>
t_ptr<t_spectra_generator> generate_generators (t_count generator_count,
                                                t_count repeat_count,
                                                T & meta_gen,
                                                std::mt19937 & gen) {
    t_generator_combiner * combiner = new t_generator_combiner();


    for (int i = 0; i < generator_count; i++) {
        t_ptr<t_spectra_generator> gen_ptr(meta_gen(gen));
        t_ptr<t_spectra_generator> rep_ptr(new t_generator_repeater(gen_ptr,
                                                                    repeat_count));
        combiner->push_back(rep_ptr);
    }

    return t_ptr<t_spectra_generator> (combiner);
}
}
}

namespace std {
ostream & operator << (ostream & out, const diagnosis::benchmark::t_spectra_generator & system);
}
#endif
