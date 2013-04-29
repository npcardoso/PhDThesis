#ifndef __COMMON_INSTRUMENTATION_SOURCES_CONSTRUCT_H__
#define __COMMON_INSTRUMENTATION_SOURCES_CONSTRUCT_H__

#include "instrumentation/transaction.h"
#include "instrumentation/oracle.h"
#include "instrumentation/probe.h"

class t_construct_source {
protected:
  inline t_construct_source(){}
  t_transaction_construct::t_ptr obs1;
  t_oracle_construct::t_ptr obs2;
  t_probe_construct::t_ptr obs3;
public:
  DEFINE_BOOST_SHARED_PTRS(t_construct_source);

  virtual bool operator >> (t_transaction_construct::t_ptr & obs);
  virtual bool operator >> (t_oracle_construct::t_ptr & obs);
  virtual bool operator >> (t_probe_construct::t_ptr & obs);

};

#endif
