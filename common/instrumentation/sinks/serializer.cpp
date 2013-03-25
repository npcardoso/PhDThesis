#include "instrumentation/sinks/serializer.h"

t_serializer_observation_sink::t_serializer_observation_sink(std::ostream & out, t_serializer::t_ptr serializer): out(out), serializer(serializer) {

}

bool t_serializer_observation_sink::operator()(const t_transaction_observation::t_ptr & obs) {
  serializer->observation_header(out);  
  serializer->observation(out, *obs);  
  serializer->observation_footer(out);  
  return true;
}

bool t_serializer_observation_sink::operator()(const t_oracle_observation::t_ptr & obs) {
  serializer->observation_header(out);  
  serializer->observation(out, *obs);  
  serializer->observation_footer(out);  
  return true;
}

bool t_serializer_observation_sink::operator()(const t_probe_observation::t_ptr & obs) {
  serializer->observation_header(out);  
  serializer->observation(out, *obs);  
  serializer->observation_footer(out);  
  return true;
}
