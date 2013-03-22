#pragma once

#include "../types.h"

#include <boost/shared_ptr.hpp>

struct t_oracle_observation {
public:
  t_time_interval time;
  t_oracle_id o_id; 
  float health, confidence;

public:
  typedef boost::shared_ptr<t_oracle_observation> t_ptr;
  typedef boost::shared_ptr<const t_oracle_observation> t_const_ptr;
  
  inline t_oracle_observation(t_time_interval time, t_oracle_id o_id, float health, float confidence) : time(time), o_id(o_id), health(health), confidence(confidence) {}
  
  inline bool operator > (const t_time_interval & time) const {
    return this->time > time;
  }
  
  inline bool operator < (const t_time_interval & time) const {
    return this->time < time;
  }
};
