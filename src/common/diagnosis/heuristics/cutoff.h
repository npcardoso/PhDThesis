#ifndef __CUTOFF_H_723413270f2a2f9527793afd5b1e8807e817ae78__
#define __CUTOFF_H_723413270f2a2f9527793afd5b1e8807e817ae78__

#include "diagnosis/heuristics/heuristic.h"

#include <algorithm>

namespace diagnosis {
namespace heuristics {
class t_cutoff : public t_heuristic_filter {
public:
    t_rank_element::t_score value_cutoff;
    float lambda;

    t_cutoff (t_rank_element::t_score value_cutoff=0,
              float lambda=1);

    virtual void operator () (const structs::t_spectra & spectra,
                              t_rank_element * ret,
                              const structs::t_spectra_filter * filter=NULL) const;

    virtual std::ostream & print (std::ostream & out) const;
};
}
}
#endif