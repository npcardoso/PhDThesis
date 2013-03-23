#include "datastore/datastore.h"
#include "utils/debug.h"

#include <cassert>

t_construct_id t_datastore::register_construct(const t_construct::t_ptr &) {
  debug("Registering construct");
  return 0;
}

void t_transaction_factory::transaction_end(t_time_interval time,
                                            t_construct_id c_id) {
  debug("New Transaction Observation End");
  assert(!active_transactions.empty());
  //TODO:
  active_transactions.pop();   
}

void t_transaction_factory::observation(const t_transaction_observation::t_ptr & obs) {
  debug("New Transaction Observation");
}

void t_transaction_factory::observation(const t_oracle_observation::t_ptr & obs) {
  debug("New Oracle Observation");
}

void t_transaction_factory::observation(const t_probe_observation::t_ptr & obs) {
  debug("New Probe Observation");
}

