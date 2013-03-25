#include "instrumentation/sinks/thread_safe.h"

#include "io/json.h"

t_thread_safe_observation_sink::t_thread_safe_observation_sink(t_observation_sink::t_ptr sink): sink(sink) {

}

bool t_thread_safe_observation_sink::operator()(const t_transaction_observation::t_ptr & obs) {
  mutex.lock();
  bool ret = (*sink)(obs);
  mutex.unlock();
  return ret;
}

bool t_thread_safe_observation_sink::operator()(const t_oracle_observation::t_ptr & obs) {
  mutex.lock();
  bool ret = (*sink)(obs);
  mutex.unlock();
  return ret;
}

bool t_thread_safe_observation_sink::operator()(const t_probe_observation::t_ptr & obs) {
  mutex.lock();
  bool ret = (*sink)(obs);
  mutex.unlock();
  return ret;
}
