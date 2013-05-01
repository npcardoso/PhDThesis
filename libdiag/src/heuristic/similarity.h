#ifndef __HEURISTIC_SIMILARITY_H__
#define __HEURISTIC_SIMILARITY_H__

#include "heuristic.h"

namespace heuristics {

class t_similarity: public t_heuristic_filter {

protected:
  virtual t_heuristic_value similarity_coefficient(const t_count n[2][2]) const = 0;

public:
  virtual void operator()(const t_spectra & spectra, 
                          t_rank_element * ret,
                          const t_spectra_filter * filter = NULL) const;
};

class t_ochiai: public t_similarity {
protected:
  virtual t_heuristic_value similarity_coefficient(const t_count n[2][2]) const;

  virtual std::ostream & print(std::ostream & out) const;
};

}
#endif
