#ifndef __DIAGNOSIS_ALGORITHMS_SINGLE_FAULT_H__
#define __DIAGNOSIS_ALGORITHMS_SINGLE_FAULT_H__

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