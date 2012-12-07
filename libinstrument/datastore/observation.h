#pragma once

#include "../types.h"
#include "state.h"

#include <boost/shared_ptr.hpp>

struct Observation {

public:
  typedef boost::shared_ptr<Observation> ptr;
  typedef boost::shared_ptr<const Observation> const_ptr;

  State * state;
  time_interval_t time;
  probe_id_t p_id;
  alloc_id_t alloc_id;

  Observation(time_interval_t time,
              probe_id_t p_id);
  ~Observation();

  void readVariable(const void * var,
                    size_t bytes);

  size_t size() const;

  inline bool operator > (const time_interval_t & time) const {
    return this->time > time;
  }
  inline bool operator < (const time_interval_t & time) const {
    return this->time < time;
  }
};
