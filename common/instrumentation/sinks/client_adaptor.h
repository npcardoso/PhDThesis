#ifndef __COMMON_INSTRUMENTATION_SINKS_CLIENT_ADAPTOR_H__
#define __COMMON_INSTRUMENTATION_SINKS_CLIENT_ADAPTOR_H__

#include "client/client.h"
#include "instrumentation/sink.h"
#include "serialization/serializer.h"

namespace instrumentation {
namespace sinks {

class t_client_adaptor: public t_observation_sink {
public:
  DEFINE_BOOST_SHARED_PTRS(t_client_adaptor);
  
  t_client_adaptor(t_client::t_ptr client,
                   t_observation_serializer::t_ptr serializer);

  bool operator << (const t_transaction_observation::t_ptr & obs);
  bool operator << (const t_oracle_observation::t_ptr & obs);
  bool operator << (const t_probe_observation::t_ptr & obs);

private:

  t_client::t_ptr client;
  t_observation_serializer::t_ptr serializer;
};
}
}

#endif
