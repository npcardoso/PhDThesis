#ifndef __COMMON_DIAGNOSIS_COMPONENT_KERNEL_H__
#define __COMMON_DIAGNOSIS_COMPONENT_KERNEL_H__

#include "types.h"
#include "component/density.h"

#include <boost/shared_ptr.hpp>
namespace diagnosis {
namespace component {
class t_kernel : public t_density_model {
public:
};

class t_kernel_uniform : public t_kernel {
public:
    virtual t_density operator () (const t_state & obs) const;
};

class t_kernel_gaussian : public t_kernel {
public:
    virtual t_density operator () (const t_state & obs) const;
};
}
}

#endif