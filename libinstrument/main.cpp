#include "main.h"

#include "datastore/datastore.h"
#include "io/json.h"
#include "utils/debug.h"

#include <algorithm>
#include <iostream>
#include <sstream>
using namespace std;

t_thread_tracker * tracker;
t_observation_sink::t_ptr * sink;

t_construct_id construct_id = 0;
boost::mutex construct_id_lock;

// observation sinks

t_observation_sink::t_ptr new_debug_json() {
  static t_observation_sink_debug::t_ptr sink(new t_observation_sink_debug (std::cout));
  return sink;
}

t_observation_sink::t_ptr new_factory() {
  return t_observation_sink::t_ptr(new t_transaction_factory(*sink));
}

void register_construct(t_construct::t_ptr construct) {
  construct_id_lock.lock();
  construct->c_id = ++construct_id;
  construct_id_lock.unlock();
}


void init() __attribute__((constructor));
void init() {
  debug("Instrumentation Init");
  
  sink = new t_observation_sink::t_ptr(new t_json_observation_sink(std::cerr));
  
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
