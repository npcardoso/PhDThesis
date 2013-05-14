#ifndef __COMMON_SERIALIZATION_SERIALIZER_H__
#define __COMMON_SERIALIZATION_SERIALIZER_H__

#include <ostream>
#include "types.h"

#include "instrumentation/sink.h"

class t_observation_serializer_options {
public:
  inline t_observation_serializer_options() {
    timestamp = true;
    cid = true;
  }
  bool timestamp;
  bool cid;
};

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

class t_observation_serializer {
protected:
  inline t_observation_serializer(){}

public:
  DEFINE_BOOST_SHARED_PTRS(t_observation_serializer);

  virtual bool operator () (std::ostream & out,
                            const instrumentation::t_oracle_observation::t_ptr & obs) = 0;
  virtual bool operator () (std::ostream & out,
                            const instrumentation::t_probe_observation::t_ptr & obs) = 0;
  virtual bool operator () (std::ostream & out,
                            const instrumentation::t_transaction_observation::t_ptr & obs) = 0;
  
  virtual t_ptr array() = 0;
  inline virtual ~t_observation_serializer(){}
  
  virtual void close(std::ostream & out) = 0;

  t_observation_serializer_options transaction_options, oracle_options, probe_options;
};

class t_construct_serializer {
protected:
  inline t_construct_serializer(){}

public:
  DEFINE_BOOST_SHARED_PTRS(t_construct_serializer);

  virtual bool operator () (std::ostream & out,
                            const instrumentation::t_oracle_construct::t_ptr & ctr) = 0;
  virtual bool operator () (std::ostream & out,
                            const instrumentation::t_probe_construct::t_ptr & ctr) = 0;
  virtual bool operator () (std::ostream & out,
                            const instrumentation::t_transaction_construct::t_ptr & ctr) = 0;
  
  virtual t_ptr array() = 0;
  virtual ~t_construct_serializer() = 0;
  
  virtual void close(std::ostream & out) = 0;
};

#endif
