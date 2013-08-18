#ifndef __DIAGNOSIS_RANDOMIZER_BERNOULLI_H__
#define __DIAGNOSIS_RANDOMIZER_BERNOULLI_H__

#include "randomizer.h"
#include "diagnosis/structs/count_spectra.h"

namespace diagnosis {
namespace randomizers {
class t_bernoulli : public t_spectra_randomizer<structs::t_count_spectra> {
public:
    t_bernoulli (float activation_rate, float error_rate);

    virtual const t_self_type & operator () (structs::t_count_spectra & spectra,
                                             structs::t_candidate & correct_candidate,
                                             boost::random::mt19937 & gen) const;

public:
    t_count n_comp;
    t_count n_tran;
    float error_rate;
    float activation_rate;
};
}
}

#endif