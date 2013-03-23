#include "threads.h"

void t_thread_tracker::start(t_thread_id thread_id) {
  mutex.lock();
  t_transaction_factory::t_ptr tmp(new t_transaction_factory());
  threads[thread_id] = tmp;
  mutex.unlock();
}

void t_thread_tracker::end(t_thread_id thread_id){
  mutex.lock();
  threads.erase(thread_id);
  mutex.unlock();
}

t_transaction_factory::t_ptr t_thread_tracker::get(t_thread_id thread_id){
  mutex.lock();
  t_transaction_factory::t_ptr tmp = threads.find(thread_id)->second;
  mutex.unlock();
  return tmp;
}

