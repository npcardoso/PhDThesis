#include "main.h"

#include "io/graphviz.h"
#include "utils/debug.h"

#include <algorithm>
#include <iostream>
#include <sstream>
using namespace std;

t_thread_tracker * tracker;

t_observation_sink::t_ptr * sink;

t_transaction_factory * new_factory() {
  return new t_transaction_factory(*sink);
}
void init() __attribute__((constructor));
void init() {
  debug("Instrumentation Init");
  
  sink = new t_observation_sink::t_ptr(new t_observation_sink_debug(std::cerr));
  
  tracker = new t_thread_tracker(new_factory);
  tracker->start();


  debug("Instrumentation Init end");
}

void finish() __attribute__((destructor));
void finish() {
//  Graphviz(cout, *getDataStore());
  delete tracker;
  debug("Instrumentation Finalize");
}
