#ifndef __DIAGNOSIS_RANDOMIZER_QOS_H__
#define __DIAGNOSIS_RANDOMIZER_QOS_H__

#include "bernoulli.h"
#include "diagnosis/structs/count_spectra.h"
#include "diagnosis/structs/topology.h"

namespace diagnosis {
namespace randomizers {
class t_fuzzy_bernoulli : public t_spectra_randomizer<structs::t_count_spectra> {
public:
    typedef std::vector<structs::t_fault> t_faults;
    DEFINE_BOOST_SHARED_PTRS(t_fuzzy_bernoulli);

    t_fuzzy_bernoulli (float activation_rate, t_count error_count);

    virtual const t_self_type & operator () (structs::t_count_spectra & spectra,
                                             structs::t_candidate & correct_candidate,
                                             boost::random::mt19937 & gen) const;

    t_fuzzy_bernoulli & operator << (const structs::t_fault & comp);

public:
    float activation_rate;
    t_count component_count;
    t_count error_count;
    t_faults faulty_components;
};
}
}

#endif