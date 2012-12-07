#include "pthread.h"

#include "../lib.h"

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

    getDataStore()->registerThread(getTimeInterval(), pthread_self(), args->parent);
    releaseDataStore();

    void * ret = args->start_routine(args->arg);

    delete args;
    getDataStore()->registerThreadEnd(getTimeInterval(), pthread_self());
    releaseDataStore();

    return ret;
  }


  void _instr_pthread_register_main(pthread_t thread) {
    getDataStore()->registerMainThread(getTimeInterval(), pthread_self());
    releaseDataStore();
  }

  int _instr_pthread_create (pthread_t * thread,
                             const pthread_attr_t *  attr,
                             void *(*start_routine) (void *),
                             void * arg) {


    pthreadArguments * args = new pthreadArguments(start_routine, arg, pthread_self());
    int ret = pthread_create(thread, attr, pthread_wrapper, args);
    if(ret)
      delete args;

    return ret;
  }

#ifdef  __cplusplus
}
#endif


