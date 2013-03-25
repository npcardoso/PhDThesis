#include "instrumentation/sinks/client.h"
#include "utils/time.h"
t_tcp_observation_sink::t_tcp_observation_sink(std::string host, std::string port, t_serializer::t_ptr serializer) {
  this->host = host;
  this->port = port;
  this->serializer = serializer;
  first = true;
}
  
t_tcp_observation_sink::~t_tcp_observation_sink() {
  cleanup();
}

void t_tcp_observation_sink::setup(bool separators){
  do {
    if(!stream.good()) {
      stream.clear();
      stream.connect(host, port);
      if (!stream)
        std::cout << "Error: " << stream.error().message() << std::endl;
    }
    if(separators){
      if(first)
        serializer->observation_request_header(stream);
      else
        serializer->observation_separator(stream);
    }
  } while(!stream.good());
  first = false;
}

void t_tcp_observation_sink::cleanup(){
  if(!first)
    do {
      if(!stream.good()) {
        stream.clear();
        stream.connect(host, port);
        if (!stream)
          std::cout << "Error: " << stream.error().message() << std::endl;
      }
      serializer->observation_request_footer(stream);
    } while(!stream.good());
  stream.close();
}

bool t_tcp_observation_sink::operator()(const t_transaction_observation::t_ptr & obs) {
  return send(obs);
}

bool t_tcp_observation_sink::operator()(const t_oracle_observation::t_ptr & obs) {
  return send(obs);
}

bool t_tcp_observation_sink::operator()(const t_probe_observation::t_ptr & obs) {
  return send(obs);
}
