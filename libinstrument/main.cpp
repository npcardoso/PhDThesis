#include "main.h"

#include "serialization/json.h"
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
//t_tcp_sink::t_ptr tcp_sink() {
//  static t_tcp_sink::t_ptr tcp_sink(new t_tcp_sink("172.30.41.220", "1234", t_json_serializer::instance()));
//  return tcp_sink;
//}

t_observation_sink::t_ptr cerr_sink() {
  static t_observation_sink::t_ptr cerr_sink(new t_json_observation_serializer(std::cerr, t_element_group_writer::t_ptr(new t_json_array())));
  return cerr_sink;
}

t_observation_sink::t_ptr thread_safe_sink() {
  static t_observation_sink::t_ptr thread_safe_sink(new t_thread_safe_observation_sink(cerr_sink()));
  return thread_safe_sink;
}

t_observation_sink::t_ptr transaction_factory_sink() {
  return t_observation_sink::t_ptr(new t_transaction_factory(thread_safe_sink()));
}

void init() __attribute__((constructor));
void init() {
  tracker = new t_thread_tracker(transaction_factory_sink);
  tracker->start();
}

void finish() __attribute__((destructor));
void finish() {
  delete tracker;
}
