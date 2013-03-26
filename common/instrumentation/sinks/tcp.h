#ifndef __COMMON_INSTRUMENTATION_SINKS_TCP_H__
#define __COMMON_INSTRUMENTATION_SINKS_TCP_H__

#include "instrumentation/serialization/serializer.h"
#include "instrumentation/sinks/construct.h"
#include "instrumentation/sinks/observation.h"

#include <boost/asio.hpp>
#include <sstream>

class t_tcp_sink: public t_observation_sink, public t_construct_sink {
  typedef enum {NONE=0, OBSERVATION=1, CONSTRUCT=2} t_request_type;
  
  std::string host, port;

  t_serializer::t_ptr serializer;
  
  boost::asio::ip::tcp::iostream stream;
  
  t_request_type current_request_type;

  void setup(t_request_type type, bool separators = true);

  void cleanup();
  
  template <class OBS>
    bool observation_send(OBS & obs) {
      do {
        setup(OBSERVATION);

        serializer->observation_header(stream);  
        serializer->observation(stream, *obs);  
        serializer->observation_footer(stream);  
      } while(!stream.good());
      return true;
    }

  template <class CTR>
    bool construct_send(CTR & ctr) {
      return true;
      do {
        setup(CONSTRUCT);

        serializer->construct_header(stream);  
        serializer->construct(stream, *ctr);  
        serializer->construct_footer(stream);  
      } while(!stream.good());
      return true;
    }


public:
  typedef boost::shared_ptr<t_tcp_sink> t_ptr;
  typedef boost::shared_ptr<const t_tcp_sink> t_const_ptr;
  
  t_tcp_sink(std::string host, std::string port, t_serializer::t_ptr serializer);
  ~t_tcp_sink();

  virtual bool operator()(const t_transaction_observation::t_ptr & obs);
  virtual bool operator()(const t_oracle_observation::t_ptr & obs);
  virtual bool operator()(const t_probe_observation::t_ptr & obs);

  virtual bool operator()(const t_transaction_construct::t_ptr & obs);
  virtual bool operator()(const t_oracle_construct::t_ptr & obs);
  virtual bool operator()(const t_probe_construct::t_ptr & obs);

};

#endif
