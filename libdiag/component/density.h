#ifndef __DENSITY_H__
#define __DENSITY_H__

#include "types.h"
typedef t_state

namespace diagnosis {
class t_density_model {
public:
  virtual t_density operator ()(const t_observation & obs) const = 0;
};

class t_density_constant : public t_density_model {
private:
  t_density density;
public:
  t_density_constant(t_density density);

  virtual t_density operator ()(const t_observation & obs) const;
};

class t_density_kernel_single : public t_density_model, public std::vector<t_observation> {
private:
  typedef boost::shared_ptr<t_density_model> t_density_model_ptr;
  t_density_model_ptr kernel;

  double bandwidth;
public:

  t_density_kernel_single(t_density_model * kernel, double bandwidth);
  t_density_kernel_single(t_density_model_ptr kernel, double bandwidth);

  virtual t_density operator ()(const t_observation & obs) const;
};
}

#endif
