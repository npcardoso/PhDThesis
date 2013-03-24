#ifndef __INSTR_DEBUG_H__
#define __INSTR_DEBUG_H__

#ifdef NDEBUG
#define debug(...)
#else
#include <stdio.h>
#define debug(...) { fprintf(stderr, "Debug %s:%d -> ", __FILE__, __LINE__); fprintf(stderr, __VA_ARGS__); fputs("\n", stderr);\
fflush(stderr);}
#endif

#include "datastore/observation_sink.h"
#include "datastore/construct_sink.h"

#include <boost/thread/mutex.hpp>

class t_construct_sink_debug: public t_construct_sink {
  std::ostream & out;
  boost::mutex io_mutex;
public:
  t_construct_sink_debug(std::ostream & out);

  virtual bool operator()(const t_transaction_construct::t_ptr & obs);
  virtual bool operator()(const t_oracle_construct::t_ptr & obs);
  virtual bool operator()(const t_probe_construct::t_ptr & obs);
};

class t_observation_sink_debug: public t_observation_sink {
  std::ostream & out;
  boost::mutex io_mutex;
public:
  t_observation_sink_debug(std::ostream & out);

  virtual bool operator()(const t_transaction_observation::t_ptr & obs);
  virtual bool operator()(const t_oracle_observation::t_ptr & obs);
  virtual bool operator()(const t_probe_observation::t_ptr & obs);
};

#endif

