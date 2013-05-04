#ifndef __DIAGNOSIS_HEURISTICS_SIMILARITY_H__
#define __DIAGNOSIS_HEURISTICS_SIMILARITY_H__

#include "diagnosis/heuristic.h"

namespace diagnosis {
namespace heuristics {

class t_similarity: public t_heuristic_filter {

protected:
  virtual t_rank_element::t_rank_score similarity_coefficient(const t_count n[2][2]) const = 0;

public:
  virtual void operator()(const t_spectra & spectra, 
                          t_rank_element * ret,
                          const t_spectra_filter * filter = NULL) const;
};

class t_ochiai: public t_similarity {
protected:
  virtual t_rank_element::t_rank_score similarity_coefficient(const t_count n[2][2]) const;

  virtual std::ostream & print(std::ostream & out) const;
};

}
}
#endif
