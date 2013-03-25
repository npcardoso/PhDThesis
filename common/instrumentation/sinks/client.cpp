#include "instrumentation/sinks/client.h"
#include "utils/debug.h"
#include "utils/time.h"
t_tcp_sink::t_tcp_sink(std::string host, std::string port, t_serializer::t_ptr serializer) {
  this->host = host;
  this->port = port;
  this->serializer = serializer;
  current_request_type = NONE;
}
  
t_tcp_sink::~t_tcp_sink() {
  cleanup();
}

void t_tcp_sink::setup(t_request_type type, bool separators){
  do {
    if(!stream.good()) {
      stream.clear();
      stream.connect(host, port);
      if (!stream) {
        std::cout << "Error: " << stream.error().message() << std::endl;
        msleep(1000);
      }
    }
    debug("%d %d", current_request_type, type);
    if(current_request_type != type) {
      debug("Switching %d %d", current_request_type, type);
      if(current_request_type != NONE) {
      debug("current_request_type is not none %d", current_request_type);
        if(current_request_type == OBSERVATION)
          serializer->observation_request_footer(stream);
        else if(current_request_type == CONSTRUCT)
          serializer->construct_request_footer(stream);

        if(separators)
          serializer->request_separator(stream);
        
        if(type == OBSERVATION)
          serializer->observation_request_footer(stream);
        else if(type == CONSTRUCT)
          serializer->construct_request_footer(stream);
      }
    } 
    else{ 
      debug("Sticking with %d %d", current_request_type, type);
      if(type == OBSERVATION)
        serializer->observation_separator(stream);
      else if(type == CONSTRUCT)
        serializer->construct_separator(stream);
    }
  } while(!stream.good());
  msleep(1000);
  current_request_type = type;
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
      serializer->observation_request_footer(stream);
    } while(!stream.good());
  stream.close();
}

bool t_tcp_sink::operator()(const t_transaction_observation::t_ptr & obs) {
  return observation_send(obs);
}

bool t_tcp_sink::operator()(const t_oracle_observation::t_ptr & obs) {
  return observation_send(obs);
}

bool t_tcp_sink::operator()(const t_probe_observation::t_ptr & obs) {
  return observation_send(obs);
}

bool t_tcp_sink::operator()(const t_transaction_construct::t_ptr & ctr) {
  return construct_send(ctr);
}

bool t_tcp_sink::operator()(const t_oracle_construct::t_ptr & ctr) {
  return construct_send(ctr);
}

bool t_tcp_sink::operator()(const t_probe_construct::t_ptr & ctr) {
  return construct_send(ctr);
}
