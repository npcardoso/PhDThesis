#ifndef __OSTREAM_ADAPTOR_H_41b8c448b73f1ad8fed93bdb74f1587944b96476__
#define __OSTREAM_ADAPTOR_H_41b8c448b73f1ad8fed93bdb74f1587944b96476__

#include "instrumentation/sink.h"
#include "instrumentation/serialization/serializer.h"

namespace instrumentation {
namespace sinks {
class t_ostream_adaptor : public t_observation_sink {
public:
    t_ostream_adaptor (std::ostream & out,
                       const t_ptr<t_observation_serializer> & serializer);

    bool operator << (const t_ptr<t_transaction_observation> & obs);
    bool operator << (const t_ptr<t_oracle_observation> & obs);
    bool operator << (const t_ptr<t_probe_observation> & obs);

private:

    std::ostream & out;
    t_ptr<t_observation_serializer> serializer;
};
}
}

#endif