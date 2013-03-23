#include "main.h"

#include "io/graphviz.h"
#include "utils/debug.h"

#include <algorithm>
#include <iostream>
#include <sstream>
using namespace std;

t_thread_tracker::t_ptr tracker;

void init() __attribute__((constructor));
void init() {
  pthread_t t;

  debug("Instrumentation Init");

  tracker = t_thread_tracker::t_ptr(new t_thread_tracker());
  tracker->start();
}

void finish() __attribute__((destructor));
void finish() {
//  Graphviz(cout, *getDataStore());
//  releaseDataStore();
  debug("Instrumentation Finalize");
}
