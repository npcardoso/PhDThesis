#ifndef __SORT_H_d34dd9734cf3d6f684fa3fbe876082665ac328ea__
#define __SORT_H_d34dd9734cf3d6f684fa3fbe876082665ac328ea__

#include "diagnosis/heuristics/heuristic.h"

namespace diagnosis {
namespace heuristics {
class t_sort : public t_heuristic_filter {
public:
    virtual void operator () (const structs::t_spectra & spectra,
                              t_rank_element * ret,
                              const structs::t_spectra_filter * filter=NULL) const;

    virtual std::ostream & print (std::ostream & out) const;
};
}
}
#endif