#ifndef __COMMON_INSTRUMENTATION_SINKS_TCP_H__
#define __COMMON_INSTRUMENTATION_SINKS_TCP_H__

#include "serialization/serializer.h"
#include "instrumentation/sinks/construct.h"
#include "instrumentation/sinks/observation.h"

#include <boost/asio.hpp>
#include <sstream>

class t_tcp_sink: public t_observation_sink, public t_construct_sink {
public:
  typedef t_observation_sink::t_ptr (* t_observation_sink_fun) (std::ostream &);
  typedef t_construct_sink::t_ptr (* t_construct_sink_fun) (std::ostream &);
  typedef boost::shared_ptr<t_tcp_sink> t_ptr;
  typedef boost::shared_ptr<const t_tcp_sink> t_const_ptr;
  
  t_tcp_sink(std::string host,
             std::string port,
             t_observation_sink_fun obs_fun,
             t_construct_sink_fun constr_fun);

  ~t_tcp_sink();

  virtual bool operator << (const t_transaction_observation::t_ptr & obs);
  virtual bool operator << (const t_oracle_observation::t_ptr & obs);
  virtual bool operator << (const t_probe_observation::t_ptr & obs);

  virtual bool operator << (const t_transaction_construct::t_ptr & obs);
  virtual bool operator << (const t_oracle_construct::t_ptr & obs);
  virtual bool operator << (const t_probe_construct::t_ptr & obs);

private:
  typedef enum {NONE=0, OBSERVATION=1, CONSTRUCT=2} t_request_type;
  
  std::string host, port;
  boost::asio::ip::tcp::iostream stream;
  
  t_request_type current_request_type;

  t_observation_sink_fun observation_sink_fun;
  t_construct_sink_fun construct_sink_fun;
  
  t_observation_serializer::t_ptr observation_serializer;
  t_construct_serializer::t_ptr construct_serializer;

  void setup(t_request_type type, bool separators = true);

  void cleanup();
  
  template <class C, class SERIAL>
    bool send(C & c, SERIAL & serializer) {
      do {
        setup(OBSERVATION);
        serializer << c;
      } while(!stream.good());
      return true;
    }
};

#endif
