#include "pthread.h"

#include "main.h"
#include "utils/debug.h"
#include "utils/time.h"

#ifdef  __cplusplus
extern "C" {
#endif

  using namespace std;
  
  class pthreadArguments {
  public:
    void *(*start_routine) (void *);
    void * arg;
    pthread_t parent;

    pthreadArguments(void *(*start_routine) (void *),
                     void * arg,
                     pthread_t parent) {

      this->start_routine = start_routine;
      this->arg = arg;
      this->parent = parent;
    }

  };



  void * pthread_wrapper(void * ptr){
    pthreadArguments * args = (pthreadArguments * ) ptr;

    tracker->start();
    void * ret = NULL;
    try {
      ret = args->start_routine(args->arg);
    }
    catch (...) {
      debug("Caught exception in pthread wrapper");
    }
    delete args;
    tracker->end();

    return ret;
  }


  int _instr_pthread_create (pthread_t * thread,
                             const pthread_attr_t *  attr,
                             void *(*start_routine) (void *),
                             void * arg) {


    debug("New Transaction");
    pthreadArguments * args = new pthreadArguments(start_routine, arg, pthread_self());
    int ret = pthread_create(thread, attr, pthread_wrapper, args);
    if(ret)
      delete args;

    return ret;
  }

#ifdef  __cplusplus
}
#endif


