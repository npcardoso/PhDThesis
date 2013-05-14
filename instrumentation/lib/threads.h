#ifndef __LIBINSTRUMENT_THREADS_H__
#define __LIBINSTRUMENT_THREADS_H__

#include <instrumentation/sink.h>

#include <boost/thread/mutex.hpp>
#include <pthread.h>
#include <map>

/* Environment dependent stuff */
typedef pthread_t t_thread_id;

inline t_thread_id thread_self() {
  return pthread_self();
}
/* Environment dependent stuff */


#include "utils/debug.h"

using namespace instrumentation;

class t_thread_tracker {
  typedef t_observation_sink::t_ptr (*t_observation_sink_fun)(void);
  typedef std::map<t_thread_id, t_observation_sink::t_ptr> t_threads;
  boost::mutex mutex;
  t_threads threads;
  t_observation_sink_fun observation_sink_fun;
public:
  DEFINE_BOOST_SHARED_PTRS(t_thread_tracker);

  t_thread_tracker (t_observation_sink_fun observation_sink_fun);
  
  void start(t_thread_id thread_id = thread_self());

  void end(t_thread_id thread_id = thread_self());
  
  t_observation_sink * get(t_thread_id thread_id = thread_self());
};

#endif
