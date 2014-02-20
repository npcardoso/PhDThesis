#include "instrumentation/sinks/collector.h"
namespace instrumentation {
namespace sinks {
bool t_observation_collector::operator << (const t_ptr<t_transaction_observation> & obs) {
    transactions.push_back(obs);
    return true;
}

bool t_observation_collector::operator << (const t_ptr<t_oracle_observation> & obs) {
    oracles.push_back(obs);
    return true;
}

bool t_observation_collector::operator << (const t_ptr<t_probe_observation> & obs) {
    probes.push_back(obs);
    return true;
}
}
}