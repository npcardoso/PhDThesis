#ifndef __COMMON_INSTRUMENTATION_SERIALIZATION_SERIALIZER_H__
#define __COMMON_INSTRUMENTATION_SERIALIZATION_SERIALIZER_H__

#include <ostream>
#include "types.h"

#include "instrumentation/sinks/observation.h"
#include "instrumentation/sinks/construct.h"

class t_observation_serializer: public t_observation_sink {
public:
  typedef boost::shared_ptr<t_observation_serializer> t_ptr;
  typedef boost::shared_ptr<const t_observation_serializer> t_const_ptr;

  virtual bool operator << (const t_oracle_observation::t_ptr & obs) = 0;
  virtual bool operator << (const t_probe_observation::t_ptr & obs) = 0;
  virtual bool operator << (const t_transaction_observation::t_ptr & obs) = 0;
  
  virtual t_ptr array() = 0;
  virtual inline ~t_observation_serializer(){};
  
  virtual void close() = 0;
};

class t_construct_serializer: public t_construct_sink {
public:
  typedef boost::shared_ptr<t_construct_serializer> t_ptr;
  typedef boost::shared_ptr<const t_construct_serializer> t_const_ptr;

  virtual bool operator << (const t_oracle_construct::t_ptr & ctr) = 0;
  virtual bool operator << (const t_probe_construct::t_ptr & ctr) = 0;
  virtual bool operator << (const t_transaction_construct::t_ptr & ctr) = 0;
  
  virtual t_ptr array() = 0;
  virtual ~t_construct_serializer() = 0;
  
  virtual void close() = 0;
};

#endif
