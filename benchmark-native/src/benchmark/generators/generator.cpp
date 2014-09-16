#include "generator.h"

using namespace std;

namespace diagnostic {
namespace benchmark {
t_generator_repeater::t_generator_repeater (t_ptr<t_spectrum_generator> generator,
                                            t_count count) : generator(generator), count(count) {}

t_spectrum * t_generator_repeater::operator () (mt19937 & gen,
                                                t_candidate & correct_candidate) {
    if (!count)
        return NULL;

    count--;
    return (* generator)(gen,
                         correct_candidate);
}

ostream & t_generator_repeater::write (ostream & out) const {
    return generator->write(out);
}

t_spectrum * t_generator_combiner::operator () (std::mt19937 & gen,
                                                t_candidate & correct_candidate) {
    while (begin() != end()) {
        t_spectrum * spectrum = (**begin())(gen, correct_candidate);

        if (spectrum != NULL)
            return spectrum;

        pop_front();
    }

    return NULL;
}
}
}


namespace std {
ostream & operator << (ostream & out,
                       const diagnostic::benchmark::t_spectrum_generator & system) {
    return system.write(out);
}
}