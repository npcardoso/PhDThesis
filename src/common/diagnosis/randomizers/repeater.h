#ifndef __REPEATER_H_b0780f2e027dad7f403da1ca195452bf8440bcd2__
#define __REPEATER_H_b0780f2e027dad7f403da1ca195452bf8440bcd2__

#include "randomizer.h"

namespace diagnosis {
namespace randomizers {
class t_system_repeater : public t_system {
public:
    t_system_repeater (t_system::t_ptr system, t_count count);

    virtual structs::t_spectra * operator () (boost::random::mt19937 & gen,
                                              structs::t_candidate & correct_candidate);
    virtual std::ostream & write (std::ostream & out) const;
private:
    t_count count;
    t_system::t_ptr system;
};

class t_architecture_repeater : public t_architecture {
public:
    t_architecture_repeater (t_architecture::t_ptr arch, t_count count, t_count spectra_count);

    virtual t_system * operator () (boost::random::mt19937 & gen);

private:
    t_count count, spectra_count;
    t_architecture::t_ptr arch;
};
}
}

#endif