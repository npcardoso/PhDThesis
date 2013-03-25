#include "instrumentation/sinks/client.h"
#include "utils/time.h"
t_tcp_observation_sink::t_tcp_observation_sink(std::string host, std::string port, t_serializer::t_ptr serializer) {
  this->host = host;
  this->port = port;
  this->serializer = serializer;
}

void t_tcp_observation_sink::connect(){
  stream.clear();
  stream.connect(host, port);
  if (!stream) {
    std::cout << "Error: " << stream.error().message() << std::endl;
  } 
}

bool t_tcp_observation_sink::operator()(const t_transaction_observation::t_ptr & obs) {
  do {
    connect();
  serializer->observation_header(stream);  
  serializer->observation(stream, *obs);  
  serializer->observation_footer(stream);  
  } while(!stream.good());
  return true;
}

bool t_tcp_observation_sink::operator()(const t_oracle_observation::t_ptr & obs) {
  if(!stream.good())
    connect();
  serializer->observation_header(stream);  
  serializer->observation(stream, *obs);  
  serializer->observation_footer(stream);  
  return true;
}

bool t_tcp_observation_sink::operator()(const t_probe_observation::t_ptr & obs) {
  if(!stream.good())
    connect();
  serializer->observation_header(stream);  
  serializer->observation(stream, *obs);  
  serializer->observation_footer(stream);  
  return true;
}
