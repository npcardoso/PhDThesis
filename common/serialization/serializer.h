#ifndef __COMMON_SERIALIZATION_SERIALIZER_H__
#define __COMMON_SERIALIZATION_SERIALIZER_H__

#include <ostream>
#include "types.h"

#include "instrumentation/sink.h"

class t_element_group_writer {
protected:
  bool first;
public:
  DEFINE_BOOST_SHARED_PTRS(t_element_group_writer);
  
  inline t_element_group_writer() {
    first = true;
  }
  
  virtual inline bool empty() const {
    return first;
  }
  
  virtual inline std::ostream & put(std::ostream & out) {
    first = false;
    return out;
  }
  
  virtual inline void close(std::ostream & out) {
    first = true;
  }
};

class t_observation_serializer: public instrumentation::t_observation_sink {
protected:
  inline t_observation_serializer(){}

public:
  DEFINE_BOOST_SHARED_PTRS(t_observation_serializer);

  virtual bool operator << (const instrumentation::t_oracle_observation::t_ptr & obs) = 0;
  virtual bool operator << (const instrumentation::t_probe_observation::t_ptr & obs) = 0;
  virtual bool operator << (const instrumentation::t_transaction_observation::t_ptr & obs) = 0;
  
  virtual t_ptr array() = 0;
  inline virtual ~t_observation_serializer(){}
  
  virtual void close() = 0;
};

class t_construct_serializer: public instrumentation::t_construct_sink {
protected:
  inline t_construct_serializer(){}

public:
  DEFINE_BOOST_SHARED_PTRS(t_construct_serializer);

  virtual bool operator << (const instrumentation::t_oracle_construct::t_ptr & ctr) = 0;
  virtual bool operator << (const instrumentation::t_probe_construct::t_ptr & ctr) = 0;
  virtual bool operator << (const instrumentation::t_transaction_construct::t_ptr & ctr) = 0;
  
  virtual t_ptr array() = 0;
  virtual ~t_construct_serializer() = 0;
  
  virtual void close() = 0;
};

#endif
