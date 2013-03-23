#ifndef __INSTR_DATASTORE_H__
#define __INSTR_DATASTORE_H__

#include "datastore/probe.h"
#include "datastore/transaction.h"
#include "datastore/oracle.h"

#include "types.h"

#include <boost/shared_ptr.hpp>
#include <map>
#include <stack>
#include <string>



class t_datastore {
public:
  t_construct_id register_construct(const t_construct::t_ptr &);

  virtual void observation(const t_transaction_observation::t_ptr & obs) = 0;
};

class t_transaction_factory: public t_datastore {
  /* Initial Time */

  typedef std::stack<t_transaction_observation::t_ptr> t_stack;
  t_stack active_transactions;
public:

  void transaction_end(t_time_interval time,
                       t_construct_id c_id);
  
  virtual void observation(const t_transaction_observation::t_ptr & obs);
  
  virtual void observation(const t_oracle_observation::t_ptr & obs);

  virtual void observation(const t_probe_observation::t_ptr & obs);
};


class t_thread_tracker {
//  typedef std::map<t_thread_id, t_thread_transactions> t_threads;

//  t_threads threads;
public:

  void thread_start(t_time_interval time,
                    pthread_t thread_id = pthread_self());

  void thread_end(t_time_interval time,
                  pthread_t thread_id = pthread_self());


};

#endif
