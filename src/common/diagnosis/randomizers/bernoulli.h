#ifndef __DIAGNOSIS_SPECTRA_RANDOMIZER_BERNOULLI_H__
#define __DIAGNOSIS_SPECTRA_RANDOMIZER_BERNOULLI_H__

#include "randomizer.h"
#include "diagnosis/structs/count_spectra.h"

namespace diagnosis {
namespace randomizers {
class t_bernoulli_randomizer : public t_spectra_randomizer<structs::t_count_spectra> {
public:
    t_bernoulli_randomizer (float activation_rate, float error_rate);

    virtual void randomize (structs::t_count_spectra & spectra) const;

public:
    t_count n_comp;
    t_count n_tran;
    float error_rate;
    float activation_rate;
};
}
}

#endif