#include "main.h"

#include "io/graphviz.h"
#include "utils/debug.h"

#include <algorithm>
#include <iostream>
#include <sstream>
using namespace std;

boost::mutex tracker_mutex;
t_thread_tracker::t_ptr _tracker(new t_thread_tracker());

t_thread_tracker::t_ptr tracker() {
  tracker_mutex.lock();
  t_thread_tracker::t_ptr tmp = _tracker;
  tracker_mutex.unlock();
  return tmp;
}

void init() __attribute__((constructor));
void init() {
  tracker_mutex.lock();
  debug("Instrumentation Init");


  _tracker = t_thread_tracker::t_ptr(new t_thread_tracker());
  tracker_mutex.unlock();
  tracker()->start();
  debug("Instrumentation Init end");
}

void finish() __attribute__((destructor));
void finish() {
//  Graphviz(cout, *getDataStore());
//  releaseDataStore();
  debug("Instrumentation Finalize");
}
