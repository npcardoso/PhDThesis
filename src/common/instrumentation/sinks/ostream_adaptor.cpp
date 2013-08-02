#include "ostream_adaptor.h"

namespace instrumentation {
namespace sinks {
t_ostream_adaptor::t_ostream_adaptor (std::ostream & out,
                                      t_observation_serializer::t_ptr serializer) : out(out), serializer(serializer) {}

bool t_ostream_adaptor::operator << (const t_transaction_observation::t_ptr & obs) {
    (* serializer)(out, obs);
    return true;
}

bool t_ostream_adaptor::operator << (const t_oracle_observation::t_ptr & obs) {
    (* serializer)(out, obs);
    return true;
}

bool t_ostream_adaptor::operator << (const t_probe_observation::t_ptr & obs) {
    (* serializer)(out, obs);
    return true;
}
}
}