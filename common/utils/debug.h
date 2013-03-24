#ifndef __INSTR_DEBUG_H__
#define __INSTR_DEBUG_H__

#ifdef NDEBUG
#define debug(...)
#else
#include <stdio.h>
#define debug(...) { fprintf(stderr, "Debug %s:%d -> ", __FILE__, __LINE__); fprintf(stderr, __VA_ARGS__); fputs("\n", stderr);\
fflush(stderr);}
#endif

#include "datastore/observation.h"
#include "datastore/construct.h"

#include <boost/thread/mutex.hpp>

class t_observation_sink_debug: public t_observation_sink{
  std::ostream & out;
  boost::mutex io_mutex;
public:
  t_observation_sink_debug(std::ostream & out);

  virtual void operator()(const t_observation::t_ptr & obs);
};

#endif

