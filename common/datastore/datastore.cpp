#include "datastore/datastore.h"
#include "utils/debug.h"

#include <cassert>

t_construct_id t_datastore::register_construct(const t_construct::t_ptr &) {
  debug("Registering construct");
  return 0;
}
  
t_transaction_observation::t_ptr t_transaction_factory::transaction_end(t_time_interval time,
                                                                        t_construct_id c_id) {
  assert(num_active());
  t_transaction_observation::t_ptr tmp = transactions.top();
  transactions.pop();   
  tmp->time_end = time;
  tmp->c_id_end = c_id;
  return tmp;
}

size_t t_transaction_factory::num_active() const {
  return transactions.size();
}

void t_transaction_factory::observation(const t_transaction_observation::t_ptr & obs) {
  if(num_active())
    transactions.top()->observation(obs);
  transactions.push(obs);
}

void t_transaction_factory::observation(const t_oracle_observation::t_ptr & obs) {
  assert(num_active());
  transactions.top()->observation(obs);
}

void t_transaction_factory::observation(const t_probe_observation::t_ptr & obs) {
  assert(num_active());
  transactions.top()->observation(obs);
}

