#ifndef __COMMON_DATASTORE_DATASTORE_H__
#define __COMMON_DATASTORE_DATASTORE_H__

#include "datastore/probe.h"
#include "datastore/transaction.h"
#include "datastore/oracle.h"

#include "types.h"

#include <map>
#include <stack>
#include <string>


class t_transaction_factory {
  typedef std::stack<t_transaction_observation::t_ptr> t_stack;
  t_stack transactions;
  t_observation_sink::t_ptr sink;

public:
  typedef boost::shared_ptr<t_transaction_factory> t_ptr;
  typedef boost::shared_ptr<const t_transaction_factory> t_const_ptr;

  t_transaction_factory(t_observation_sink::t_ptr sink = t_observation_sink::t_ptr());

  t_transaction_observation::t_ptr transaction_end(t_time_interval time,
                                                   t_construct_id c_id);

  size_t num_active() const;

  virtual void observation(const t_transaction_observation::t_ptr & obs);
  
  virtual void observation(const t_oracle_observation::t_ptr & obs);

  virtual void observation(const t_probe_observation::t_ptr & obs);
};

#endif
