#include "ostream_adaptor.h"

namespace instrumentation {
namespace sinks {
t_ostream_adaptor::t_ostream_adaptor (std::ostream & out,
                                      const t_ptr<t_observation_serializer> & serializer) : out(out), serializer(serializer) {}

bool t_ostream_adaptor::operator << (const t_ptr<t_transaction_observation> & obs) {
    (* serializer)(out, obs);
    return true;
}

bool t_ostream_adaptor::operator << (const t_ptr<t_oracle_observation> & obs) {
    (* serializer)(out, obs);
    return true;
}

bool t_ostream_adaptor::operator << (const t_ptr<t_probe_observation> & obs) {
    (* serializer)(out, obs);
    return true;
}
}
}