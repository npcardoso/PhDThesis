#ifndef __LIBINSTRUMENT_THREADS_H__
#define __LIBINSTRUMENT_THREADS_H__

#include <datastore/datastore.h>

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

class t_thread_tracker {
  typedef t_transaction_factory * (*t_factory_fun)(void);
  typedef std::map<t_thread_id, t_transaction_factory::t_ptr> t_threads;
  boost::mutex mutex;
  t_threads threads;
  t_factory_fun factory_fun;
public:
  typedef boost::shared_ptr<t_thread_tracker> t_ptr;
  typedef boost::shared_ptr<const t_thread_tracker> t_const_ptr;

  t_thread_tracker (t_factory_fun factory_fun);
  
  void start(t_thread_id thread_id = thread_self());

  void end(t_thread_id thread_id = thread_self());
  
  t_transaction_factory::t_ptr get(t_thread_id thread_id = thread_self());
};

#endif
