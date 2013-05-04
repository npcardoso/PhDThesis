#include "instrumentation/sinks/transaction_factory.h"

namespace instrumentation {
namespace sinks {

t_transaction_factory::t_transaction_factory(t_observation_sink::t_ptr sink):sink(sink){
}
  
size_t t_transaction_factory::num_active() const {
  return transactions.size();
}

bool t_transaction_factory::operator << (const t_transaction_observation::t_ptr & obs) {
  if((!obs->c_id_start) && (!obs->c_id_end))
    return false;

  if(obs->c_id_start) { // Push action
    if(num_active())
      transactions.top()->observation(obs);
    transactions.push(obs);
  }
  
  if(!num_active()) 
    return false;

  if(obs->c_id_end) { // Pop action
      t_transaction_observation::t_ptr tmp = transactions.top();
      transactions.pop();   

      tmp->time_end = obs->time_end;
      tmp->c_id_end = obs->c_id_end;

      if(sink && !num_active())  // Forward
        return (*sink) << tmp;
      return false;
  }
  
  return true;
}

bool t_transaction_factory::operator << (const t_oracle_observation::t_ptr & obs) {
  if(num_active()) {
    transactions.top()->observation(obs);
    return true;
  }

  if(sink) // Forward
    return (*sink) << obs;
  return false;
}

bool t_transaction_factory::operator << (const t_probe_observation::t_ptr & obs) {
  if(num_active()) {
    transactions.top()->observation(obs);
    return true;
  }

  if(sink) // Forward
    return(*sink) << obs;
  return false;
}
}
}
