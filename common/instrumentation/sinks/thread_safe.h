#ifndef __COMMON_INSTRUMENTATION_SINKS_THREAD_SAFE_H__
#define __COMMON_INSTRUMENTATION_SINKS_THREAD_SAFE_H__

#include "instrumentation/sinks/observation.h"
#include <boost/thread/mutex.hpp>

class t_thread_safe_observation_sink: public t_observation_sink {
  boost::mutex mutex;
  t_observation_sink::t_ptr sink;
public:
  t_thread_safe_observation_sink(t_observation_sink::t_ptr sink);

  virtual bool operator()(const t_transaction_observation::t_ptr & obs);
  virtual bool operator()(const t_oracle_observation::t_ptr & obs);
  virtual bool operator()(const t_probe_observation::t_ptr & obs);
};

#endif
