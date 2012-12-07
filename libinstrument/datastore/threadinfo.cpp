#include "threadinfo.h"

ThreadInfo::ThreadInfo(time_interval_t start, thread_id_t parent_id) {
  this->start = start;
  this->end = -start;
  this->parent_id = parent_id;
}

void ThreadInfo::addObservation(Observation::const_ptr obs){
  assert(observations.empty() || obs->time > observations.back()->time);
  observations.push_back(obs);
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

class ObsCompareTime{
public:
  bool operator() (time_interval_t time, Observation::const_ptr obs){
    return (*obs) > time;
  }
  bool operator() (Observation::const_ptr obs, time_interval_t time){
    return (*obs) < time;
  }
};
ThreadInfo::observation_storage_t::const_iterator ThreadInfo::getObservationsAfter(time_interval_t time, bool include) const {
  if(include)
    return lower_bound(observations.begin(), observations.end(), time, ObsCompareTime());
  else
    return upper_bound(observations.begin(), observations.end(), time, ObsCompareTime());
}

