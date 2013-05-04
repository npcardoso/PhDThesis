#include "instrumentation/sinks/collector.h"
namespace instrumentation {
namespace sinks {

bool t_observation_collector_sink::operator << (const t_transaction_observation::t_ptr & obs) {
  transactions.push_back(obs);
  return true;
}

bool t_observation_collector_sink::operator << (const t_oracle_observation::t_ptr & obs) {
  oracles.push_back(obs);
  return true;
}

bool t_observation_collector_sink::operator << (const t_probe_observation::t_ptr & obs) {
  probes.push_back(obs);
  return true;
}
}
}
