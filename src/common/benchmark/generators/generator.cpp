#include "generator.h"

using namespace diagnosis::structs;
using namespace std;

namespace diagnosis {
namespace benchmark {
t_generator_repeater::t_generator_repeater (t_ptr<t_spectra_generator> generator,
                                            t_count count) : generator(generator), count(count) {}

t_spectra * t_generator_repeater::operator () (mt19937 & gen,
                                               t_candidate & correct_candidate) {
    if (!count)
        return NULL;

    count--;
    return (* generator)(gen, correct_candidate);
}

ostream & t_generator_repeater::write (ostream & out) const {
    return generator->write(out);
}

structs::t_spectra * t_generator_combiner::operator () (std::mt19937 & gen,
                                                        structs::t_candidate & correct_candidate) {
    while (begin() != end()) {
        t_spectra * spectra = (**begin())(gen, correct_candidate);

        if (spectra != NULL)
            return spectra;

        pop_front();
    }

    return NULL;
}
}
}


namespace std {
ostream & operator << (ostream & out, const diagnosis::benchmark::t_spectra_generator & system) {
    return system.write(out);
}
}