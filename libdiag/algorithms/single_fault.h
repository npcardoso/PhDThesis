#ifndef __SINGLE_FAULT_H_7fc4474ddd10cf6152746ce230d2e4124ccbed4d__
#define __SINGLE_FAULT_H_7fc4474ddd10cf6152746ce230d2e4124ccbed4d__

#include "../diagnostic_system.h"

namespace diagnostic {
namespace algorithms {
class t_single_fault : public t_candidate_generator {
public:
    virtual void operator () (const structs::t_spectra & spectra,
                              t_ret_type & D,
                              const structs::t_spectra_filter * filter=NULL) const;

    inline virtual std::string to_string() const {
        return "t_single_fault";
    }

};
}
}

#endif
