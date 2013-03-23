#include "datastore/datastore.h"
#include "utils/debug.h"

#include <cassert>


void t_thread_tracker::thread_start(t_time_interval time, pthread_t thread_id) {
}

void t_thread_tracker::thread_end(t_time_interval time, pthread_t pthread_id){
}

void t_transaction_factory::transaction_end(t_time_interval time,
                                            t_construct_id c_id) {
  debug("New Transaction Observation End");
  assert(!active_transactions.empty());
  //TODO:
  active_transactions.pop();   
}

void t_transaction_factory::observation(const t_oracle_observation::t_ptr & obs) {
  debug("New Oracle Observation");
}

void t_transaction_factory::observation(const t_probe_observation::t_ptr & obs) {
  debug("New Probe Observation");
}

