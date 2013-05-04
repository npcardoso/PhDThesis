#ifndef __COMMON_INSTRUMENTATION_SINKS_COLLECTOR_H__
#define __COMMON_INSTRUMENTATION_SINKS_COLLECTOR_H__

#include "instrumentation/sink.h"

namespace instrumentation {
namespace sinks {

class t_observation_collector_sink: public t_observation_sink {
public:
  DEFINE_BOOST_SHARED_PTRS(t_observation_collector_sink);
  
  bool operator << (const t_transaction_observation::t_ptr & obs);
  bool operator << (const t_oracle_observation::t_ptr & obs);
  bool operator << (const t_probe_observation::t_ptr & obs);
  
  std::list<t_transaction_observation::t_ptr> transactions;
  std::list<t_probe_observation::t_ptr> probes;
  std::list<t_oracle_observation::t_ptr> oracles;
};
}
}

#endif
