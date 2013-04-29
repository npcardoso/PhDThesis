#ifndef __COMMON_INSTRUMENTATION_ORACLE_H__
#define __COMMON_INSTRUMENTATION_ORACLE_H__

#include "types.h"

#include "instrumentation/construct.h"
#include "instrumentation/observation.h"

class t_oracle_construct: public t_construct {
public:
  DEFINE_BOOST_SHARED_PTRS(t_oracle_construct);
};

class t_oracle_observation: public t_observation_single {
public:
  float health, confidence;

public:
  DEFINE_BOOST_SHARED_PTRS(t_oracle_observation);
  
  inline t_oracle_observation(t_time_interval time, t_construct_id c_id, float health, float confidence) : t_observation_single(time, c_id), health(health), confidence(confidence) {}
  
  virtual size_t size() const;
};

#endif
