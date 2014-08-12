#ifndef __MHS_CREATOR_H_22238d508e71484c52d786e4203b584cb3a85135__
#define __MHS_CREATOR_H_22238d508e71484c52d786e4203b584cb3a85135__

#include "algorithm_creator.h"

namespace diagnostic {
namespace configuration {

class t_mhs_creator: public t_algorithm_creator {
    public:
    virtual t_const_ptr<t_candidate_generator> generator (const t_algorithm & a) const;
};
}
}

#endif
