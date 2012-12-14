#pragma once

#include "../types.h"
#include "observation.h"
#include "transaction.h"
#include "oracleresult.h"

#include <boost/shared_ptr.hpp>
#include <vector>
#include <stack>

using namespace std;

class ThreadInfo {
public:
  typedef boost::shared_ptr<ThreadInfo> ptr;
  typedef vector<Observation::const_ptr> observation_storage_t;
  typedef vector<Transaction::const_ptr> transaction_storage_t;
  typedef vector<OracleResult::const_ptr> oracle_result_storage_t;

  time_interval_t start, end;

  thread_id_t parent_id;

  observation_storage_t observations;
  transaction_storage_t transactions;
  oracle_result_storage_t oracle_results;

  stack<Transaction::ptr> transaction_stack;

  ThreadInfo(time_interval_t launch_time,
             thread_id_t parent_id);

  void addObservation(Observation::const_ptr obs);
  
  void addOracleResult(OracleResult::const_ptr o_res);

  void pushTransaction();
  void pushTransaction(Transaction::ptr tr);
  void popTransaction(time_interval_t time,
                      transaction_gate_id_t gate);

  observation_storage_t::const_iterator getObservationsAfter(time_interval_t time, bool include=false) const;
  
  oracle_result_storage_t::const_iterator getOracleResultsAfter(time_interval_t time, bool include=false) const;

  inline bool ended() const {
    return start < end;
  }
};

