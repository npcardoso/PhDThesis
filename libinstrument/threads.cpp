#include "threads.h"

#include "utils/debug.h"

t_thread_tracker::t_thread_tracker (t_factory_fun factory_fun) : factory_fun(factory_fun) {

}

void t_thread_tracker::start(t_thread_id thread_id) {
  mutex.lock();
  
  t_threads::iterator it = threads.find(thread_id);
  assert(it == threads.end());
  
  t_transaction_factory::t_ptr tmp(factory_fun());
  threads[thread_id] = tmp;
  
  mutex.unlock();
}

void t_thread_tracker::end(t_thread_id thread_id){
  mutex.lock();
 
  t_threads::iterator it = threads.find(thread_id);
  assert(it != threads.end());
  
  threads.erase(thread_id);
  
  mutex.unlock();
}

t_transaction_factory::t_ptr t_thread_tracker::get(t_thread_id thread_id){
  mutex.lock();
  
  t_threads::iterator it = threads.find(thread_id);
  assert(it != threads.end());

  t_transaction_factory::t_ptr tmp = it->second;
  
  mutex.unlock();
  return tmp;
}

