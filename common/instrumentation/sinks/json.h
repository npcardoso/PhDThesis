#ifndef __COMMON_INSTRUMENTATION_SINKS_JSON_H__
#define __COMMON_INSTRUMENTATION_SINKS_JSON_H__

#include "instrumentation/sinks/observation.h"

class t_json_observation_sink: public t_observation_sink {
  std::ostream & out;
public:
  t_json_observation_sink(std::ostream & out);

  virtual bool operator()(const t_transaction_observation::t_ptr & obs);
  virtual bool operator()(const t_oracle_observation::t_ptr & obs);
  virtual bool operator()(const t_probe_observation::t_ptr & obs);
};

#endif
