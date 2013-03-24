#ifndef __COMMON_INSTRUMENTATION_ORACLE_H__
#define __COMMON_INSTRUMENTATION_ORACLE_H__

#include "types.h"

#include "instrumentation/construct.h"
#include "instrumentation/observation.h"

class t_oracle_construct: public t_construct {
public:
  typedef boost::shared_ptr<t_oracle_construct> t_ptr;
  typedef boost::shared_ptr<const t_oracle_construct> t_const_ptr;
};

class t_oracle_observation: public t_observation_single {
public:
  float health, confidence;

public:
  typedef boost::shared_ptr<t_oracle_observation> t_ptr;
  typedef boost::shared_ptr<const t_oracle_observation> t_const_ptr;
  
  inline t_oracle_observation(t_time_interval time, t_construct_id c_id, float health, float confidence) : t_observation_single(time, c_id), health(health), confidence(confidence) {}
  
  virtual size_t size() const;
};

#endif
