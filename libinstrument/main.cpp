#include "main.h"

#include "instrumentation/serialization/json.h"
#include "instrumentation/sinks/serializer.h"
#include "instrumentation/sinks/client.h"
#include "instrumentation/sinks/thread_safe.h"
#include "instrumentation/sinks/transaction_factory.h"
#include "utils/debug.h"

#include <algorithm>
#include <iostream>
#include <sstream>
using namespace std;

t_thread_tracker * tracker;

t_construct_id construct_id = 0;
boost::mutex mutex;

// observation sinks

t_observation_sink::t_ptr json_sink() {
  static t_observation_sink::t_ptr serializer_sink(new t_serializer_observation_sink(std::cerr, t_json_serializer::instance()));
  static t_observation_sink::t_ptr tcp_sink(new t_tcp_observation_sink("localhost", "1234", t_json_serializer::instance()));
  static t_observation_sink::t_ptr thread_safe_sink(new t_thread_safe_observation_sink(tcp_sink));
  static t_observation_sink::t_ptr sink(thread_safe_sink);
  
  return sink;
}

t_observation_sink::t_ptr transaction_factory_sink() {
  return t_observation_sink::t_ptr(new t_transaction_factory(json_sink()));
}

void register_construct(t_construct::t_ptr construct) {
  mutex.lock();
  construct->c_id = ++construct_id;
  mutex.unlock();
}


void init() __attribute__((constructor));
void init() {
  debug("Instrumentation Init");
  
  tracker = new t_thread_tracker(transaction_factory_sink);
  tracker->start();


  debug("Instrumentation Init end");
}

void finish() __attribute__((destructor));
void finish() {
//  Graphviz(cout, *getDataStore());
  delete tracker;
  debug("Instrumentation Finalize");
}
