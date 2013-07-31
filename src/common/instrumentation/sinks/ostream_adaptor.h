#ifndef __COMMON_INSTRUMENTATION_OSTREAM_ADAPTOR_H__
#define __COMMON_INSTRUMENTATION_OSTREAM_ADAPTOR_H__

#include "instrumentation/sink.h"
#include "serialization/serializer.h"

namespace instrumentation {
namespace sinks {
class t_ostream_adaptor : public t_observation_sink {
public:
    DEFINE_BOOST_SHARED_PTRS(t_ostream_adaptor);

    t_ostream_adaptor (std::ostream & out,
                       t_observation_serializer::t_ptr serializer);

    bool operator << (const t_transaction_observation::t_ptr & obs);
    bool operator << (const t_oracle_observation::t_ptr & obs);
    bool operator << (const t_probe_observation::t_ptr & obs);

private:

    std::ostream & out;
    t_observation_serializer::t_ptr serializer;
};
}
}

#endif