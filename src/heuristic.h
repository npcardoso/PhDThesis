#ifndef __HEURISTIC_H__
#define __HEURISTIC_H__

#include "spectra.h"
#include "spectra_filter.h"

template <class T_ACTIVITY>
class t_heuristic {

public:
  virtual void order(const t_spectra <T_ACTIVITY> & spectra, 
                     const t_spectra_filter * filter, 
                     t_component_id * ret) const = 0;
};


#endif
