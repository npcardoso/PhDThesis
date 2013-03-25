#ifndef __COMMON_INSTRUMENTATION_SINKS_CLIENT_H__
#define __COMMON_INSTRUMENTATION_SINKS_CLIENT_H__

#include "instrumentation/serialization/serializer.h"
#include "instrumentation/sinks/observation.h"

#include <boost/asio.hpp>
#include <sstream>

class t_tcp_observation_sink: public t_observation_sink {
  std::string host, port;

  t_serializer::t_ptr serializer;
  boost::asio::ip::tcp::iostream stream;
  std::stringstream ss;
  
  void connect();
public:
  t_tcp_observation_sink(std::string host, std::string port, t_serializer::t_ptr serializer);

  virtual bool operator()(const t_transaction_observation::t_ptr & obs);
  virtual bool operator()(const t_oracle_observation::t_ptr & obs);
  virtual bool operator()(const t_probe_observation::t_ptr & obs);
};

#endif
