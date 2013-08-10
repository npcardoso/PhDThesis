#ifndef __DIAGNOSIS_RANDOMIZER_QOS_H__
#define __DIAGNOSIS_RANDOMIZER_QOS_H__

#include "bernoulli.h"
#include "diagnosis/structs/count_spectra.h"

namespace diagnosis {
namespace randomizers {
class t_faulty_component {
public:
    t_faulty_component (t_error e_min, t_error e_max);

    t_error gen_error (boost::random::mt19937 & gen) const;
public:
    float e_min, e_max;
};

class t_qos_randomizer : public t_bernoulli_randomizer {
public:
    typedef std::vector<t_faulty_component> t_faulty_components;
    DEFINE_BOOST_SHARED_PTRS(t_qos_randomizer);

    t_qos_randomizer (float activation_rate, float error_rate);

    virtual void operator () (structs::t_count_spectra & spectra,
                              structs::t_candidate & correct_candidate,
                              boost::random::mt19937 & gen) const;

    t_qos_randomizer & operator << (const t_faulty_component & comp);

public:
    t_faulty_components faulty_components;
};
}
}

#endif