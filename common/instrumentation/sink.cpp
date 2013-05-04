#include "instrumentation/sink.h"
namespace instrumentation {

bool t_observation_sink::operator << (const t_transaction_observation::t_ptr & obs) {
  return false;
}

bool t_observation_sink::operator << (const t_oracle_observation::t_ptr & obs) {
  return false;
}

bool t_observation_sink::operator << (const t_probe_observation::t_ptr & obs) {
  return false;
}

bool t_construct_sink::operator << (const t_transaction_construct::t_ptr & obs) {
  return false;
}

bool t_construct_sink::operator << (const t_oracle_construct::t_ptr & obs) {
  return false;
}

bool t_construct_sink::operator << (const t_probe_construct::t_ptr & obs) {
  return false;
}
}


