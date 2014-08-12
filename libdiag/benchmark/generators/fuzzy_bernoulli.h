#ifndef __FUZZY_BERNOULLI_H_a53881ae3d9ecc98bea674a957037d2b0c5e7dcd__
#define __FUZZY_BERNOULLI_H_a53881ae3d9ecc98bea674a957037d2b0c5e7dcd__

#include "generator.h"
#include "../../structs/topology.h"


namespace diagnostic {
namespace benchmark {
class t_fuzzy_bernoulli : public t_spectra_generator {
public:
    typedef std::vector<structs::t_fault> t_faults;

    t_fuzzy_bernoulli  (t_count component_count,
                        float activation_rate,
                        t_count error_count);

    virtual structs::t_spectra * operator () (std::mt19937 & gen,
                                              structs::t_candidate & correct_candidate);

    t_fuzzy_bernoulli & operator << (const structs::t_fault & comp);

    virtual std::ostream & write (std::ostream & out) const;

public:
    float activation_rate;
    t_count component_count;
    t_count error_count;
    t_faults faulty_components;
};
}
}

#endif
