#ifndef __MHS_CREATOR_H_22238d508e71484c52d786e4203b584cb3a85135__
#define __MHS_CREATOR_H_22238d508e71484c52d786e4203b584cb3a85135__

#include "algorithm_creator.h"
#include "../algorithms/mhs.h"

namespace diagnostic {
namespace configuration {

class t_mhs_creator: public t_algorithm_creator {
    public:
    t_mhs_creator();
    virtual t_const_ptr<t_candidate_generator> generator (const t_algorithm & a) const;

    private:
    t_ptr<algorithms::t_cutoff> default_cutoff;
    t_const_ptr<algorithms::t_similarity> default_similarity;
};
}
}

#endif
