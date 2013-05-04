#ifndef __COMMON_INSTRUMENTATION_SINKS_DEVNULL_H__
#define __COMMON_INSTRUMENTATION_SINKS_DEVNULL_H__

#include "instrumentation/sink.h"

namespace instrumentation {
namespace sinks {

class t_dev_null_sink: public t_observation_sink, public t_construct_sink {
public:
  DEFINE_BOOST_SHARED_PTRS(t_dev_null_sink);
  
  inline bool operator << (const t_transaction_observation::t_ptr & obs) {
    return true;
  }

  inline bool operator << (const t_oracle_observation::t_ptr & obs) {
    return true;
  }

  inline bool operator << (const t_probe_observation::t_ptr & obs) {
    return true;
  }


  inline bool operator << (const t_transaction_construct::t_ptr & obs) {
    return true;
  }

  inline bool operator << (const t_oracle_construct::t_ptr & obs) {
    return true;
  }

  inline bool operator << (const t_probe_construct::t_ptr & obs) {
    return true;
  }
};
}
}

#endif
