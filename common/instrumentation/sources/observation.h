#ifndef __COMMON_INSTRUMENTATION_SOURCES_OBSERVATION_H__
#define __COMMON_INSTRUMENTATION_SOURCES_OBSERVATION_H__

#include "instrumentation/transaction.h"
#include "instrumentation/oracle.h"
#include "instrumentation/probe.h"

class t_observation_source {
protected:
  inline t_observation_source(){}

public:
  DEFINE_BOOST_SHARED_PTRS(t_observation_source);

  virtual bool operator >> (t_transaction_observation::t_ptr & obs);
  virtual bool operator >> (t_oracle_observation::t_ptr & obs);
  virtual bool operator >> (t_probe_observation::t_ptr & obs);
};

#endif
