#ifndef __COMMON_SERIALIZATION_UNSERIALIZER_H__
#define __COMMON_SERIALIZATION_UNSERIALIZER_H__

#include <ostream>
#include "types.h"

#include "instrumentation/sources/observation.h"
#include "instrumentation/sources/construct.h"

class t_observation_unserializer: public t_observation_source {
protected:
  inline t_observation_unserializer(){}

public:
  DEFINE_BOOST_SHARED_PTRS(t_observation_unserializer);

  virtual bool operator >> (t_oracle_observation::t_ptr & obs) = 0;
  virtual bool operator >> (t_probe_observation::t_ptr & obs) = 0;
  virtual bool operator >> (t_transaction_observation::t_ptr & obs) = 0;
  
  virtual bool ended() const = 0;
  
  inline virtual t_observation_id get_tid() const {
    return 0;
  }

  inline virtual t_observation_id get_pid() const {
    return 0;
  }
  
  inline virtual ~t_observation_unserializer(){}
};

class t_construct_unserializer: public t_construct_source {
protected:
  inline t_construct_unserializer(){}
public:
  DEFINE_BOOST_SHARED_PTRS(t_construct_unserializer);

  virtual bool operator >> (t_oracle_construct::t_ptr & ctr) = 0;
  virtual bool operator >> (t_probe_construct::t_ptr & ctr) = 0;
  virtual bool operator >> (t_transaction_construct::t_ptr & ctr) = 0;
  
  virtual bool ended() const = 0;
  
  virtual ~t_construct_unserializer() = 0;
};

#endif
