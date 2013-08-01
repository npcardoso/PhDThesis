#ifndef __DIAGNOSIS_SPECTRA_RANDOMIZER_BERNOULLI_H__
#define __DIAGNOSIS_SPECTRA_RANDOMIZER_BERNOULLI_H__

#include "randomizer.h"
#include "diagnosis/spectra/count_spectra.h"

namespace diagnosis {
class t_bernoulli_randomizer : public t_spectra_randomizer < t_count_spectra > {
public:
    t_bernoulli_randomizer (float activation_rate, float error_rate);

    virtual void randomize (t_count_spectra & spectra) const;

public:
    t_count n_comp;
    t_count n_tran;
    float error_rate;
    float activation_rate;
};
}

#endif