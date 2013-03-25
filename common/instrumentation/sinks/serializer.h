#ifndef __COMMON_INSTRUMENTATION_SINKS_SERIALIZER_H__
#define __COMMON_INSTRUMENTATION_SINKS_SERIALIZER_H__

#include "instrumentation/sinks/observation.h"
#include "instrumentation/serialization/serializer.h"

class t_serializer_observation_sink: public t_observation_sink {
  std::ostream & out;
  t_serializer::t_ptr serializer;
public:
  t_serializer_observation_sink(std::ostream & out, t_serializer::t_ptr serializer);

  virtual bool operator()(const t_transaction_observation::t_ptr & obs);
  virtual bool operator()(const t_oracle_observation::t_ptr & obs);
  virtual bool operator()(const t_probe_observation::t_ptr & obs);
};

#endif
