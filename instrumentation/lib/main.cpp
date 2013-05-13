#include "main.h"

#include "serialization/serializers/json.h"
#include "instrumentation/sinks/ostream_adaptor.h"
#include "instrumentation/sinks/thread_safe.h"
#include "instrumentation/sinks/transaction_factory.h"
#include "utils/debug.h"

#include <algorithm>
#include <iostream>
#include <sstream>

using namespace std;
using namespace instrumentation;
using namespace instrumentation::sinks;

t_observation_sink::t_ptr configure_sink() {
  t_observation_serializer::t_ptr json_serializer(new t_json_observation_serializer());
  t_observation_sink::t_ptr ostream_adaptor(new t_ostream_adaptor(std::cerr, json_serializer));
  t_observation_sink::t_ptr thread_safe_sink(new t_thread_safe_observation(ostream_adaptor));
  return thread_safe_sink;
}

t_observation_sink::t_ptr sink = configure_sink();
t_thread_tracker * tracker;

t_construct_id construct_id = 0;
boost::mutex mutex;

t_observation_sink::t_ptr transaction_factory_sink() {
  return t_observation_sink::t_ptr(new t_transaction_factory(sink));
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
