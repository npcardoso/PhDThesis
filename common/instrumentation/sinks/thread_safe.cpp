#include "instrumentation/sinks/thread_safe.h"

t_thread_safe_construct_sink::t_thread_safe_construct_sink(t_construct_sink::t_ptr sink): sink(sink) {

}

bool t_thread_safe_construct_sink::operator << (const t_transaction_construct::t_ptr & ctr) {
  return protect(ctr);
}

bool t_thread_safe_construct_sink::operator << (const t_oracle_construct::t_ptr & ctr) {
  return protect(ctr);
}

bool t_thread_safe_construct_sink::operator << (const t_probe_construct::t_ptr & ctr) {
  return protect(ctr);
}

t_thread_safe_observation_sink::t_thread_safe_observation_sink(t_observation_sink::t_ptr sink): sink(sink) {

}

bool t_thread_safe_observation_sink::operator << (const t_transaction_observation::t_ptr & obs) {
  return protect(obs);
}

bool t_thread_safe_observation_sink::operator << (const t_oracle_observation::t_ptr & obs) {
  return protect(obs);
}

bool t_thread_safe_observation_sink::operator << (const t_probe_observation::t_ptr & obs) {
  return protect(obs);
}
