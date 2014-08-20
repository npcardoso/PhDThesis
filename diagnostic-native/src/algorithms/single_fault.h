#ifndef __SINGLE_FAULT_H_7fc4474ddd10cf6152746ce230d2e4124ccbed4d__
#define __SINGLE_FAULT_H_7fc4474ddd10cf6152746ce230d2e4124ccbed4d__

#include "../diagnostic_system.h"

namespace diagnostic {
namespace algorithms {
class t_single_fault : public t_candidate_generator {
public:
    virtual void operator () (const t_spectra & spectra,
                              t_ret_type & D,
                              const t_spectra_filter * filter=NULL) const;

    inline virtual std::string get_name() const {
        return "single_fault";
    }

    inline virtual void json_configs (t_configs & out) const {}

};
}
}

#endif
