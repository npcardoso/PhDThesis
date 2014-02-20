#ifndef __COMMON_INSTRUMENTATION_SINKS_CLIENT_ADAPTOR_H__
#define __COMMON_INSTRUMENTATION_SINKS_CLIENT_ADAPTOR_H__

#include "client/client.h"
#include "instrumentation/sink.h"
#include "serialization/serializer.h"

namespace instrumentation {
namespace sinks {
class t_client_adaptor : public t_observation_sink {
public:
    DEFINE_BOOST_SHARED_PTRS(t_client_adaptor);

    t_client_adaptor (t_ptr<t_client> client,
                      t_ptr<t_observation_serializer> serializer);

    bool operator << (const t_ptr<t_transaction_observation> & obs);
    bool operator << (const t_ptr<t_oracle_observation> & obs);
    bool operator << (const t_ptr<t_probe_observation> & obs);

private:

    t_ptr<t_client> client;
    t_ptr<t_observation_serializer> serializer;
};
}
}

#endif