#include "main.h"

#include "client/tcp.h"
#include "serialization/serializers/json.h"
#include "instrumentation/transaction.h"
#include "instrumentation/sinks/client_adaptor.h"
#include "instrumentation/sinks/ostream_adaptor.h"
#include "instrumentation/sinks/devnull.h"
#include "instrumentation/sinks/thread_safe.h"
#include "instrumentation/sinks/transaction_factory.h"
#include "utils/debug.h"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
using namespace instrumentation;
using namespace instrumentation::sinks;

t_observation_sink::t_ptr configure_sink() {
  t_observation_serializer::t_ptr json_serializer(new t_json_observation_serializer());
  t_observation_sink::t_ptr sink(new t_dev_null());

  json_serializer->transaction_options.timestamp = false;
  json_serializer->transaction_options.cid = false;
  json_serializer->oracle_options.timestamp = false;
  json_serializer->oracle_options.cid = false;
  json_serializer->probe_options.timestamp = false;
  json_serializer->probe_options.cid = true;
  
  if(char * server = std::getenv("INSTRUMENTATION_SERVER")) {
    char * port = "12345";
    if(char * port_str = std::getenv("INSTRUMENTATION_SERVER_PORT"))
      port = port_str;
    t_client::t_ptr client(new t_tcp_client(server, port));
    sink = t_observation_sink::t_ptr(new t_client_adaptor(client, json_serializer));
  }
  else if(char * devnull = std::getenv("INSTRUMENTATION_STDERR")) {
    sink = t_observation_sink::t_ptr(new t_ostream_adaptor(std::cerr, json_serializer));
  }
  else if(char * file = std::getenv("INSTRUMENTATION_FILE")) {
    static std::fstream f(file);
    sink = t_observation_sink::t_ptr(new t_ostream_adaptor(f, json_serializer));
  }
  t_observation_sink::t_ptr thread_safe_sink(new t_thread_safe_observation(sink));
  return thread_safe_sink;
}

t_observation_sink::t_ptr sink = configure_sink();

t_observation_sink::t_ptr transaction_factory_sink() {
  return t_observation_sink::t_ptr(new t_transaction_factory(sink));
}


t_thread_tracker::t_ptr init_tracker() {
  t_thread_tracker::t_ptr tracker(new t_thread_tracker(transaction_factory_sink));
  tracker->start();
  return tracker;
}

t_thread_tracker::t_ptr tracker = init_tracker();

t_construct_id construct_id = 0;
boost::mutex mutex;
