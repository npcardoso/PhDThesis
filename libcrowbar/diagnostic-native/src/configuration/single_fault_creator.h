#ifndef __SINGLE_FAULT_CREATOR_H_22eba728f526cd66f44fe195ca65d34e7ddbd4d5__
#define __SINGLE_FAULT_CREATOR_H_22eba728f526cd66f44fe195ca65d34e7ddbd4d5__

#include "algorithm_creator.h"


namespace diagnostic {
namespace configuration {

class t_single_fault_creator : public t_algorithm_creator {
    public:
    virtual t_const_ptr<t_candidate_generator> generator(const t_algorithm & a) const;
};
}
}

#endif
