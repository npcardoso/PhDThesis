#pragma once

#include "../types.h"
#include "probe.h"
#include "transaction.h"
#include "oracle.h"

#include <boost/shared_ptr.hpp>
#include <vector>
#include <stack>

using namespace std;

class t_thread_info {
public:
  typedef boost::shared_ptr<t_thread_info> t_ptr;
  typedef vector<t_probe_observation::t_const_ptr> t_probe_storage;
  typedef vector<t_transaction_observation::t_const_ptr> t_transaction_storage;
  typedef vector<t_oracle_observation::t_const_ptr> t_oracle_storage;

  t_time_interval start, end;

  t_thread_id parent_id;

  t_probe_storage probes;
  t_transaction_storage transactions;
  t_oracle_storage oracles;

  stack<t_transaction_observation::t_ptr> transaction_stack;

  t_thread_info(t_time_interval launch_time,
                t_thread_id parent_id);

  bool observation(t_probe_observation::t_const_ptr obs);
  bool observation(t_oracle_observation::t_const_ptr obs);

  void push_transaction();
  void push_transaction(t_transaction_observation::t_ptr tr);
  void pop_transaction(t_time_interval time,
                       t_transaction_gate_id gate);

  t_probe_storage::const_iterator probes_after(t_time_interval time, bool include=false) const;

  t_oracle_storage::const_iterator oracles_after(t_time_interval time, bool include=false) const;

  inline bool ended() const {
    return start < end;
  }
};

