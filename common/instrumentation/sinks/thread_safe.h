#ifndef __COMMON_INSTRUMENTATION_SINKS_THREAD_SAFE_H__
#define __COMMON_INSTRUMENTATION_SINKS_THREAD_SAFE_H__

#include "instrumentation/sink.h"
#include <boost/thread/mutex.hpp>
namespace instrumentation {
namespace sinks {

class t_thread_safe_construct_sink: public t_construct_sink {
  boost::mutex mutex;
  t_construct_sink::t_ptr sink;
  
  template <class CTR>
    bool protect(CTR & ctr) {
      mutex.lock();
      bool ret = (*sink) << ctr;
      mutex.unlock();
      return ret;
    }
public:
  t_thread_safe_construct_sink(t_construct_sink::t_ptr sink);
  
  virtual bool operator << (const t_transaction_construct::t_ptr & ctr);
  virtual bool operator << (const t_oracle_construct::t_ptr & ctr);
  virtual bool operator << (const t_probe_construct::t_ptr & ctr);

};

class t_thread_safe_observation_sink: public t_observation_sink {
  boost::mutex mutex;
  t_observation_sink::t_ptr sink;
  
  template <class OBS>
    bool protect(OBS & obs) {
      mutex.lock();
      bool ret = (*sink) << obs;
      mutex.unlock();
      return ret;
    }
public:
  t_thread_safe_observation_sink(t_observation_sink::t_ptr sink);

  virtual bool operator << (const t_transaction_observation::t_ptr & obs);
  virtual bool operator << (const t_oracle_observation::t_ptr & obs);
  virtual bool operator << (const t_probe_observation::t_ptr & obs);
};
}
}

#endif
