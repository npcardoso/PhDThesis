#ifndef __HEURISTIC_PARALLELIZATION_H__
#define __HEURISTIC_PARALLELIZATION_H__

#include "heuristic.h"

namespace heuristics {

template <class T_ACTIVITY>
  class t_divide: public t_heuristic_filter<T_ACTIVITY> {
    t_count division, division_count, stride;
  public:
    t_divide(t_count division, 
                    t_count division_count, 
                    t_count stride = 1) {
      assert(division < division_count);
      assert(division_count > 0);
      assert(stride > 0);
      this->division = division;
      this->division_count = division_count;
      this->stride = stride;
    }

    virtual void operator()(const t_spectra <T_ACTIVITY> & spectra, 
                            t_rank_element * ret,
                            const t_spectra_filter * filter = NULL) const {
      t_count component_count = spectra.get_component_count(filter);

      for(t_id i = 0; i < component_count; i++)
        if((i / stride) % division_count != division)
          ret[i] = t_rank_element(ret[i].get_component(), -1);
    }

    virtual std::ostream & print(std::ostream & out) const {
      return out << "t_divide(d:" << division << ", #d:"  << division_count << ", s:"  << stride << ")";
    }

  };

}
#endif
