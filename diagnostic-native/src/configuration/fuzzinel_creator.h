#ifndef __FUZZINEL_CREATOR_H_9495a66437765ddc9ec8284942ce20d750f12bf4__
#define __FUZZINEL_CREATOR_H_9495a66437765ddc9ec8284942ce20d750f12bf4__

#include "algorithm_creator.h"

namespace diagnostic {
namespace configuration {

class t_fuzzinel_creator: public t_algorithm_creator {
    public:
    virtual t_const_ptr<t_candidate_ranker> ranker (const t_algorithm & a) const;
};
}
}


#endif
