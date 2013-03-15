#ifndef __KERNEL_H__
#define __KERNEL_H__

#include "density.h"
#include "../types.h"

#include <boost/shared_ptr.hpp>

class t_kernel_uniform : public t_density_model {
public:
  virtual t_density operator ()(const t_observation & obs) const;
};

class t_kernel_gaussian : public t_density_model {
public:
  virtual t_density operator ()(const t_observation & obs) const;
};

#endif
