#ifndef __COMMON_INSTRUMENTATION_SINKS_OBSERVATION_H__
#define __COMMON_INSTRUMENTATION_SINKS_OBSERVATION_H__

#include "instrumentation/transaction.h"
#include "instrumentation/oracle.h"
#include "instrumentation/probe.h"

class t_observation_sink {
protected:
  inline t_observation_sink(){}

public:
  DEFINE_BOOST_SHARED_PTRS(t_observation_sink);

  virtual bool operator << (const t_transaction_observation::t_ptr & obs);
  virtual bool operator << (const t_oracle_observation::t_ptr & obs);
  virtual bool operator << (const t_probe_observation::t_ptr & obs);
};

#endif
