#ifndef __COMMON_INSTRUMENTATION_PROBE_H__
#define __COMMON_INSTRUMENTATION_PROBE_H__

#include "types.h"

#include "instrumentation/construct.h"
#include "instrumentation/observation.h"

class t_probe_construct: public t_construct {
public:
  DEFINE_BOOST_SHARED_PTRS(t_probe_construct);
};

struct t_state {
public:
  unsigned char * data;
  size_t * offset_end;
  size_t n_vars;

public:
  t_state();
  ~t_state();

  void read_variable(const void * var,
                     size_t bytes);

  inline size_t data_size() const {
    if(n_vars)
      return offset_end[n_vars - 1];
    else
      return 0;
  }

  inline size_t size() const {
    return sizeof(size_t) * n_vars + data_size();
  }
};

struct t_probe_observation: public t_observation_single {

public:
  DEFINE_BOOST_SHARED_PTRS(t_probe_observation);

  t_state * state;

  t_probe_observation(t_time_interval time,
                      t_construct_id c_id);
  ~t_probe_observation();

  void read_variable(const void * var,
                     size_t bytes);

  virtual size_t size() const;
};

#endif
