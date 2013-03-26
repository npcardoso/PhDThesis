#include "instrumentation/sinks/observation.h"

bool t_observation_sink::operator << (const t_transaction_observation::t_ptr & obs) {
  return false;
}

bool t_observation_sink::operator << (const t_oracle_observation::t_ptr & obs) {
  return false;
}

bool t_observation_sink::operator << (const t_probe_observation::t_ptr & obs) {
  return false;
}

