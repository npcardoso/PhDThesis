#include "datastore/threadinfo.h"

t_thread_info::t_thread_info(t_time_interval start, t_thread_id parent_id) {
  this->start = start;
  this->end = -start;
  this->parent_id = parent_id;
}

bool t_thread_info::observation(t_probe_observation::t_const_ptr obs){
  assert(probes.empty() || obs->time > probes.back()->time);
  if(transaction_stack.empty())  //Drop observations that are not related to any transaction
    return false;

  probes.push_back(obs);
  return true;
}

bool t_thread_info::observation(t_oracle_observation::t_const_ptr obs){
  assert(oracles.empty() || obs->time > oracles.back()->time);
  if(transaction_stack.empty())  //Drop observations that are not related to any transaction
    return false;

  oracles.push_back(obs);
  return true;
}

void t_thread_info::push_transaction(){
  transaction_stack.push(t_transaction_observation::t_ptr());
}

void t_thread_info::push_transaction(t_transaction_observation::t_ptr tr){
  assert(tr != NULL);
  assert(transactions.empty() || tr->start > transactions.back()->start);
  assert(transaction_stack.empty() || tr->start > transaction_stack.top()->start);
  assert(!tr->ended());

  transaction_stack.push(tr);
  transactions.push_back(tr);
}

void t_thread_info::pop_transaction(t_time_interval time,
                                t_transaction_gate_id gate){

  assert(!transaction_stack.empty());

  t_transaction_observation::t_ptr tr = transaction_stack.top();
  transaction_stack.pop();
  if(tr != NULL) {
    tr->end = time;
    tr->gate_end = gate;
  }
}

template <class T>
class TimeCompare {
public:
  bool operator() (t_time_interval time, typename T::t_const_ptr obj){
    return (*obj) > time;
  }
  bool operator() (typename T::t_const_ptr obj, t_time_interval time){
    return (*obj) < time;
  }
};

t_thread_info::t_probe_storage::const_iterator t_thread_info::probes_after(t_time_interval time, bool include) const {
  if(include)
    return lower_bound(probes.begin(), probes.end(), time, TimeCompare<t_probe_observation>());
  else
    return upper_bound(probes.begin(), probes.end(), time, TimeCompare<t_probe_observation>());
}

t_thread_info::t_oracle_storage::const_iterator t_thread_info::oracles_after(t_time_interval time, bool include) const {
  if(include)
    return lower_bound(oracles.begin(), oracles.end(), time, TimeCompare<t_oracle_observation>());
  else
    return upper_bound(oracles.begin(), oracles.end(), time, TimeCompare<t_oracle_observation>());
}

