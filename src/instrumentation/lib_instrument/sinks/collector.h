#ifndef __COMMON_INSTRUMENTATION_SINKS_COLLECTOR_H__
#define __COMMON_INSTRUMENTATION_SINKS_COLLECTOR_H__

#include "instrumentation/sink.h"

namespace instrumentation {
namespace sinks {
class t_observation_collector : public t_observation_sink {
public:
    bool operator << (const t_ptr<t_transaction_observation> & obs);
    bool operator << (const t_ptr<t_oracle_observation> & obs);
    bool operator << (const t_ptr<t_probe_observation> & obs);

    std::list<t_ptr<t_transaction_observation> > transactions;
    std::list<t_ptr<t_probe_observation> > probes;
    std::list<t_ptr<t_oracle_observation> > oracles;
};
}
}

#endif