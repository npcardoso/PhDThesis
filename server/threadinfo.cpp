#include "threadinfo.h"

ThreadInfo::ThreadInfo(time_interval_t start, thread_id_t parent_id) {
  this->start = start;
  this->end = -start;
  this->parent_id = parent_id;
}

bool ThreadInfo::addObservation(Observation::const_ptr obs){
  assert(observations.empty() || obs->time > observations.back()->time);
  if(transaction_stack.empty())  //Drop observations that are not related to any transaction
    return false;

  observations.push_back(obs);
  return true;
}

bool ThreadInfo::addOracleResult(OracleResult::const_ptr o_res){
  assert(oracle_results.empty() || o_res->time > oracle_results.back()->time);
  if(transaction_stack.empty())  //Drop oracle results that are not related to any transaction
    return false;

  oracle_results.push_back(o_res);
  return true;
}

void ThreadInfo::pushTransaction(){
  transaction_stack.push(Transaction::ptr());
}

void ThreadInfo::pushTransaction(Transaction::ptr tr){
  assert(tr != NULL);
  assert(transactions.empty() || tr->start > transactions.back()->start);
  assert(transaction_stack.empty() || tr->start > transaction_stack.top()->start);
  assert(!tr->ended());

  transaction_stack.push(tr);
  transactions.push_back(tr);
}

void ThreadInfo::popTransaction(time_interval_t time,
                                transaction_gate_id_t gate){

  assert(!transaction_stack.empty());

  Transaction::ptr tr = transaction_stack.top();
  transaction_stack.pop();
  if(tr != NULL) {
    tr->end = time;
    tr->gate_end = gate;
  }
}

template <class T>
class TimeCompare {
public:
  bool operator() (time_interval_t time, typename T::const_ptr obj){
    return (*obj) > time;
  }
  bool operator() (typename T::const_ptr obj, time_interval_t time){
    return (*obj) < time;
  }
};

ThreadInfo::observation_storage_t::const_iterator ThreadInfo::getObservationsAfter(time_interval_t time, bool include) const {
  if(include)
    return lower_bound(observations.begin(), observations.end(), time, TimeCompare<Observation>());
  else
    return upper_bound(observations.begin(), observations.end(), time, TimeCompare<Observation>());
}

ThreadInfo::oracle_result_storage_t::const_iterator ThreadInfo::getOracleResultsAfter(time_interval_t time, bool include) const {
  if(include)
    return lower_bound(oracle_results.begin(), oracle_results.end(), time, TimeCompare<OracleResult>());
  else
    return upper_bound(oracle_results.begin(), oracle_results.end(), time, TimeCompare<OracleResult>());
}

