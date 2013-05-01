#include "kernel.h"

#include <boost/math/distributions/normal.hpp>

t_density t_kernel_uniform::operator ()(const t_observation & obs) const {
  if(obs * obs <= 1)
    return 0.5;
  return 0;
}

t_density t_kernel_gaussian::operator ()(const t_observation & obs) const {
  return(boost::math::pdf(boost::math::normal(), obs));
}
