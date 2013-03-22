#pragma once

#include "../types.h"
#include "state.h"

#include <boost/shared_ptr.hpp>

struct t_probe_observation {

public:
  typedef boost::shared_ptr<t_probe_observation> t_ptr;
  typedef boost::shared_ptr<const t_probe_observation> t_const_ptr;

  t_state * state;
  t_time_interval time;
  t_probe_id p_id;
  t_alloc_id alloc_id;

  t_probe_observation(t_time_interval time,
                      t_probe_id p_id);
  ~t_probe_observation();

  void read_variable(const void * var,
                     size_t bytes);

  size_t size() const;

  inline bool operator > (const t_time_interval & time) const {
    return this->time > time;
  }
  inline bool operator < (const t_time_interval & time) const {
    return this->time < time;
  }
};
