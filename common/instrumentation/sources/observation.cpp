#include "instrumentation/sources/observation.h"

bool t_observation_source::operator >> (t_transaction_observation::t_ptr & obs) {
  return false;
}

bool t_observation_source::operator >> (t_oracle_observation::t_ptr & obs) {
  return false;
}

bool t_observation_source::operator >> (t_probe_observation::t_ptr & obs) {
  return false;
}

