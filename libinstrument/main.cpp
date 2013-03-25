#include "main.h"

#include "instrumentation/sinks/json.h"
#include "instrumentation/sinks/thread_safe.h"
#include "instrumentation/sinks/transaction_factory.h"
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

t_observation_sink::t_ptr json_sink() {
  static t_json_observation_sink::t_ptr sink(new t_json_observation_sink(std::cout));
  static t_json_observation_sink::t_ptr sink_thread_safe(new t_thread_safe_observation_sink(sink));
  return sink_thread_safe;
}

t_observation_sink::t_ptr transaction_factory_sink() {
  return t_observation_sink::t_ptr(new t_transaction_factory(json_sink()));
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
  
  tracker = new t_thread_tracker(json_sink);
  tracker->start();


  debug("Instrumentation Init end");
}

void finish() __attribute__((destructor));
void finish() {
//  Graphviz(cout, *getDataStore());
  delete tracker;
  debug("Instrumentation Finalize");
}
