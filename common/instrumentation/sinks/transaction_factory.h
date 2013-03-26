#ifndef __COMMON_INSTRUMENTATION_SINKS_TRANSACTION_FACTORY_H__
#define __COMMON_INSTRUMENTATION_SINKS_TRANSACTION_FACTORY_H__

#include "instrumentation/sinks/observation.h"

#include "types.h"

#include <map>
#include <stack>
#include <string>


class t_transaction_factory: public t_observation_sink {
  typedef std::stack<t_transaction_observation::t_ptr> t_stack;
  t_stack transactions;
  t_observation_sink::t_ptr sink;

public:
  typedef boost::shared_ptr<t_transaction_factory> t_ptr;
  typedef boost::shared_ptr<const t_transaction_factory> t_const_ptr;

  t_transaction_factory(t_observation_sink::t_ptr sink = t_observation_sink::t_ptr());

  size_t num_active() const;

  virtual bool operator << (const t_transaction_observation::t_ptr & obs);
  
  virtual bool operator << (const t_oracle_observation::t_ptr & obs);

  virtual bool operator << (const t_probe_observation::t_ptr & obs);
};

#endif
