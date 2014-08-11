#ifndef __BERNOULLI_H_a6d1be206c8818d13562fb163cc8b337ff1a0f31__
#define __BERNOULLI_H_a6d1be206c8818d13562fb163cc8b337ff1a0f31__

#include "generator.h"

namespace diagnosis {
namespace benchmark {
class t_bernoulli : public t_spectra_generator {
public:
    t_bernoulli (float activation_rate,
                 float error_rate,
                 t_count n_tran,
                 t_count n_comp);

    virtual structs::t_spectra * operator () (std::mt19937 & gen,
                                              structs::t_candidate & correct_candidate);

    virtual std::ostream & write (std::ostream & out) const;

public:
    t_count n_comp;
    t_count n_tran;
    float error_rate;
    float activation_rate;
};
}
}

#endif