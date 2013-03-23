#ifndef __COMMON_DATASTORE_TRANSACTION_H__
#define __COMMON_DATASTORE_TRANSACTION_H__

#include "types.h"

#include "datastore/construct.h"
#include "datastore/observation.h"
#include "datastore/probe.h"
#include "datastore/oracle.h"

#include <list>

class t_transaction_construct: public t_construct {};

struct t_transaction_observation: public t_observation_window{

public:
  typedef boost::shared_ptr<t_transaction_observation> t_ptr;
  typedef boost::shared_ptr<const t_transaction_observation> t_const_ptr;
  
  typedef std::list<t_ptr> t_transactions;
  typedef std::list<t_oracle_observation::t_ptr> t_oracles;
  typedef std::list<t_probe_observation::t_ptr> t_probes;

  t_transactions transactions;
  t_oracles oracles;
  t_probes probes;

  inline t_transaction_observation(t_time_interval time,
                                   t_construct_id c_id): t_observation_window(time, c_id) {}
  
  virtual void observation(const t_transaction_observation::t_ptr & obs);
  virtual void observation(const t_oracle_observation::t_ptr & obs);
  virtual void observation(const t_probe_observation::t_ptr & obs);
  
  virtual size_t size() const;
};

#endif
