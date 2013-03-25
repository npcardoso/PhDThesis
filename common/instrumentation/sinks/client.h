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
  bool first;  
  void setup(bool separators = true);
  void cleanup();
  template <class OBS>
    bool send(OBS & obs) {
      do {
        setup();

        serializer->observation_header(stream);  
        serializer->observation(stream, *obs);  
        serializer->observation_footer(stream);  
      } while(!stream.good());
      return true;
    }


public:
  t_tcp_observation_sink(std::string host, std::string port, t_serializer::t_ptr serializer);
  ~t_tcp_observation_sink();

  virtual bool operator()(const t_transaction_observation::t_ptr & obs);
  virtual bool operator()(const t_oracle_observation::t_ptr & obs);
  virtual bool operator()(const t_probe_observation::t_ptr & obs);

};

#endif
