#include "repeater.h"
namespace diagnosis {
namespace randomizers {
t_system_repeater::t_system_repeater (t_ptr<t_system> system,
                                      t_count count) : system(system), count(count) {}

structs::t_spectra * t_system_repeater::operator () (boost::random::mt19937 & gen,
                                                     structs::t_candidate & correct_candidate) {
    if (!count)
        return NULL;

    count--;
    return (* system)(gen, correct_candidate);
}

std::ostream & t_system_repeater::write (std::ostream & out) const {
    return system->write(out);
}

t_architecture_repeater::t_architecture_repeater (t_ptr<t_architecture> arch,
                                                  t_count count,
                                                  t_count spectra_count) : arch(arch), count(count), spectra_count(spectra_count) {}

t_system * t_architecture_repeater::operator () (boost::random::mt19937 & gen) {
    if (!count)
        return NULL;

    count--;
    t_ptr<t_system> tmp((* arch)(gen));
    return new t_system_repeater(tmp, spectra_count);
}
}
}