#ifndef __COMMON_DATASTORE_CONSTRUCT_SINK_H__
#define __COMMON_DATASTORE_CONSTRUCT_SINK_H__

#include "datastore/transaction.h"
#include "datastore/oracle.h"
#include "datastore/probe.h"

class t_construct_sink {
protected:
  inline t_construct_sink(){}
  t_transaction_construct::t_ptr obs1;
  t_oracle_construct::t_ptr obs2;
  t_probe_construct::t_ptr obs3;
public:
  typedef boost::shared_ptr<t_construct_sink> t_ptr;
  typedef boost::shared_ptr<const t_construct_sink> t_const_ptr;

  virtual bool operator()(const t_transaction_construct::t_ptr & obs);
  virtual bool operator()(const t_oracle_construct::t_ptr & obs);
  virtual bool operator()(const t_probe_construct::t_ptr & obs);

};

#endif
