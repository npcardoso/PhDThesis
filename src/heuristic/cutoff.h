#ifndef __CUTOFF_H__
#define __CUTOFF_H__

#include "heuristic.h"

#include <algorithm>

template <class T_ACTIVITY>
class t_filter_cutoff: public t_heuristic_filter <T_ACTIVITY> {
public:
  t_heuristic_value value_cutoff;
  float lambda;
  
  t_filter_cutoff(t_heuristic_value value_cutoff = 0, float lambda = 1) {
    this->value_cutoff = value_cutoff;
    this->lambda = lambda;
  }

  virtual void operator()(const t_spectra <T_ACTIVITY> & spectra, 
                          t_rank_element * ret,
                          const t_spectra_filter * filter = NULL) const {

    t_count component_count = spectra.get_component_count(filter);
    t_count max_component = lambda * component_count;
    max_component = std::min(max_component, component_count);

    t_id i = 0;
    while(i < max_component) {
      t_heuristic_value value = ret[i].get_value();
      if(value >= 0 && value < value_cutoff)
        ret[i] = t_rank_element (0, -1);
      i++;
    }
    
    while(i < component_count)
      ret[i++] = t_rank_element (0, -1);
  }
};


#endif

