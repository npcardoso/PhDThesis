#include "instrumentation/sinks/json.h"

#include "io/json.h"

t_json_observation_sink::t_json_observation_sink(std::ostream & out): out(out) {

}

bool t_json_observation_sink::operator()(const t_transaction_observation::t_ptr & obs) {
  out << '{';
  t_json::observation(out, *obs);  
  out << '}';
  return true;
}

bool t_json_observation_sink::operator()(const t_oracle_observation::t_ptr & obs) {
  out << '{';
  t_json::observation(out, *obs);  
  out << '}';
  return true;
}

bool t_json_observation_sink::operator()(const t_probe_observation::t_ptr & obs) {
  out << '{';
  t_json::observation(out, *obs);  
  out << '}';
  return true;
}
