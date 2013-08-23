#ifndef __BERNOULLI_H_a6d1be206c8818d13562fb163cc8b337ff1a0f31__
#define __BERNOULLI_H_a6d1be206c8818d13562fb163cc8b337ff1a0f31__

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