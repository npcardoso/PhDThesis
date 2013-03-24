#include "datastore/datastore.h"
#include "utils/debug.h"

#include <cassert>

t_transaction_factory::t_transaction_factory(t_observation_sink::t_ptr sink):sink(sink){
}
  
t_transaction_observation::t_ptr t_transaction_factory::transaction_end(t_time_interval time,
                                                                        t_construct_id c_id) {
  assert(num_active());
  t_transaction_observation::t_ptr tmp = transactions.top();
  transactions.pop();   
  
  tmp->time_end = time;
  tmp->c_id_end = c_id;
  
  if(sink)
    (*sink)(tmp);
  
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
  if(num_active())
    transactions.top()->observation(obs);
  else if(sink)
    (*sink)(obs);
}

void t_transaction_factory::observation(const t_probe_observation::t_ptr & obs) {
  if(num_active())
    transactions.top()->observation(obs);
  else if(sink)
    (*sink)(obs);
}

