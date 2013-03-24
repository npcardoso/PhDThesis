#include "datastore/datastore.h"
#include "utils/debug.h"

#include <cassert>

t_transaction_factory::t_transaction_factory(t_observation_sink::t_ptr sink):sink(sink){
}
  
size_t t_transaction_factory::num_active() const {
  return transactions.size();
}

bool t_transaction_factory::operator()(const t_transaction_observation::t_ptr & obs) {
  if(obs->c_id_start) { // Push action
    if(num_active())
      transactions.top()->observation(obs);
    transactions.push(obs);
    return true;
  }

  t_transaction_observation::t_ptr tmp = obs;

  if(num_active()) { // Pop action
    tmp = transactions.top();
    transactions.pop();   

    tmp->time_end = obs->time_end;
    tmp->c_id_end = obs->c_id_end;
  }
  
  if(sink) // Forward
    return (*sink)(tmp);
  return false;
}

bool t_transaction_factory::operator()(const t_oracle_observation::t_ptr & obs) {
  if(num_active()) {
    transactions.top()->observation(obs);
    return true;
  }

  if(sink) // Forward
    return (*sink)(obs);
  return false;
}

bool t_transaction_factory::operator()(const t_probe_observation::t_ptr & obs) {
  if(num_active()) {
    transactions.top()->observation(obs);
    return true;
  }

  if(sink) // Forward
    return(*sink)(obs);
  return false;
}
