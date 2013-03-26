#include "instrumentation/sinks/tcp.h"

#include "utils/debug.h"
#include "utils/time.h"

t_tcp_sink::t_tcp_sink(std::string host,
                       std::string port,
                       t_observation_sink_fun obs_fun,
                       t_construct_sink_fun constr_fun) {
  this->host = host;
  this->port = port;
  this->observation_sink_fun = obs_fun;
  this->construct_sink_fun = constr_fun;
  current_request_type = NONE;
  stream.connect(host, port);
}

t_tcp_sink::~t_tcp_sink() {
  cleanup();
}

void t_tcp_sink::setup(t_request_type type, bool separators){
  while(!stream.good()) {
    stream.clear();
    stream.connect(host, port);
    if (!stream) {
      std::cout << "Error: " << stream.error().message() << std::endl;
      msleep(1000);
    }
  }

  if(current_request_type != type) {
    observation_serializer->close();
    construct_serializer->close();
    current_request_type = type;
  }
}

void t_tcp_sink::cleanup(){
  if(current_request_type)
    do {
      if(!stream.good()) {
        stream.clear();
        stream.connect(host, port);
        if (!stream)
          std::cout << "Error: " << stream.error().message() << std::endl;
      }
      observation_serializer->close();
      construct_serializer->close();
    } while(!stream.good());
  stream.close();
}

bool t_tcp_sink::operator << (const t_transaction_observation::t_ptr & obs) {
  return send(obs, *observation_serializer);
}

bool t_tcp_sink::operator << (const t_oracle_observation::t_ptr & obs) {
  return send(obs, *observation_serializer);
}

bool t_tcp_sink::operator << (const t_probe_observation::t_ptr & obs) {
  return send(obs, *observation_serializer);
}

bool t_tcp_sink::operator << (const t_transaction_construct::t_ptr & ctr) {
  return send(ctr, *construct_serializer);
}

bool t_tcp_sink::operator << (const t_oracle_construct::t_ptr & ctr) {
  return send(ctr, *construct_serializer);
}

bool t_tcp_sink::operator << (const t_probe_construct::t_ptr & ctr) {
  return send(ctr, *construct_serializer);
}
