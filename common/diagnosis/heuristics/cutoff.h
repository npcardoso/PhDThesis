#ifndef __DIAGNOSIS_HEURISTICS_CUTOFF_H__
#define __DIAGNOSIS_HEURISTICS_CUTOFF_H__

#include "diagnosis/heuristic.h"

#include <algorithm>

namespace diagnosis {
namespace heuristics {

class t_cutoff: public t_heuristic_filter {
public:
  t_heuristic_value value_cutoff;
  float lambda;

  t_cutoff(t_heuristic_value value_cutoff = 0, float lambda = 1);

  virtual void operator()(const t_spectra & spectra, 
                          t_rank_element * ret,
                          const t_spectra_filter * filter = NULL) const;

  virtual std::ostream & print(std::ostream & out) const;
};

}
}
#endif

