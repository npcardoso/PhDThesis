#include "density.h"

t_density_constant::t_density_constant(t_density density): density(density) {
}

t_density t_density_constant::operator ()(const t_observation & obs) const {
  return density;
}

t_density t_density_kernel_single::operator ()(const t_observation & obs) const {
  t_density density = 0;
  
  for(const_iterator it = begin(); it != end(); it++)
    density += (*kernel)((obs - *it) / bandwidth);
  return  density / (bandwidth * size());
}

t_density_kernel_single::t_density_kernel_single(t_density_model * kernel, double bandwidth):kernel(kernel), bandwidth(bandwidth) {
}

t_density_kernel_single::t_density_kernel_single(t_density_kernel_single::t_density_model_ptr kernel, double bandwidth):kernel(kernel), bandwidth(bandwidth) {
}
