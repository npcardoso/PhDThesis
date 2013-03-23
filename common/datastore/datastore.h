#ifndef __COMMON_DATASTORE_DATASTORE_H__
#define __COMMON_DATASTORE_DATASTORE_H__

#include "datastore/probe.h"
#include "datastore/transaction.h"
#include "datastore/oracle.h"

#include "types.h"

#include <map>
#include <stack>
#include <string>



class t_datastore {
public:
  t_construct_id register_construct(const t_construct::t_ptr &);

  virtual void observation(const t_transaction_observation::t_ptr & obs) = 0;
};

class t_transaction_factory: public t_datastore {


  typedef std::stack<t_transaction_observation::t_ptr> t_stack;
  t_stack active_transactions;
public:
  typedef boost::shared_ptr<t_transaction_factory> t_ptr;
  typedef boost::shared_ptr<const t_transaction_factory> t_const_ptr;

  void transaction_end(t_time_interval time,
                       t_construct_id c_id);
  
  virtual void observation(const t_transaction_observation::t_ptr & obs);
  
  virtual void observation(const t_oracle_observation::t_ptr & obs);

  virtual void observation(const t_probe_observation::t_ptr & obs);
};

#endif
