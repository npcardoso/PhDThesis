#ifndef __FILTER_SORT_H__
#define __FILTER_SORT_H__

#include "heuristic.h"

#include <algorithm>

template <class T_ACTIVITY>
class t_filter_sort: public t_heuristic_filter <T_ACTIVITY> {
public:
  virtual void operator()(const t_spectra <T_ACTIVITY> & spectra, 
                          t_rank_element * ret,
                          const t_spectra_filter * filter = NULL) const {

    t_count component_count = spectra.get_component_count(filter);
    std::sort(ret, ret + component_count);
  }
  virtual std::ostream & print(std::ostream & out) const {
    return out << "t_filter_sort";
  }
};


#endif
