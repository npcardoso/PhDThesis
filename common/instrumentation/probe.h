#ifndef __COMMON_INSTRUMENTATION_PROBE_H__
#define __COMMON_INSTRUMENTATION_PROBE_H__

#include "types.h"

#include "instrumentation/construct.h"
#include "instrumentation/observation.h"
#include "instrumentation/state.h"

namespace instrumentation {

class t_probe_construct: public t_construct {
public:
  DEFINE_BOOST_SHARED_PTRS(t_probe_construct);
};

class t_probe_observation: public t_observation_single {

public:
  DEFINE_BOOST_SHARED_PTRS(t_probe_observation);

  t_state * state;

  t_probe_observation();
  t_probe_observation(t_time_interval time,
                      t_construct_id c_id);
  t_probe_observation(const t_probe_observation & p);

  ~t_probe_observation();

  void read_variable(const void * var,
                     size_t bytes);

  virtual size_t size() const;

  void clear_state();

  t_probe_observation & operator = (const t_probe_observation & p);
};
}

#endif
