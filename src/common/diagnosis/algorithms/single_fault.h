#ifndef __SINGLE_FAULT_H_7fc4474ddd10cf6152746ce230d2e4124ccbed4d__
#define __SINGLE_FAULT_H_7fc4474ddd10cf6152746ce230d2e4124ccbed4d__

#include "diagnosis/diagnosis_system.h"

namespace diagnosis {
namespace algorithms {
class t_single_fault : public t_candidate_generator {
public:
    DEFINE_BOOST_SHARED_PTRS(t_single_fault);

    virtual void operator () (const structs::t_spectra & spectra,
                              t_ret_type & D,
                              const structs::t_spectra_filter * filter=NULL) const;
};
}
}

#endif