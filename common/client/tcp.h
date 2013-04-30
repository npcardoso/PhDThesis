#ifndef __COMMON_CLIENT_TCP_H__
#define __COMMON_CLIENT_TCP_H__

#include "types.h"

#include <boost/asio.hpp>
#include <sstream>

class t_tcp_client {
public:
  DEFINE_BOOST_SHARED_PTRS(t_tcp_client);

  t_tcp_client(std::string host,
               std::string port);

  ~t_tcp_client();

  void write(const std::string & s);
private:
  void setup();
  std::string host, port;
  boost::asio::ip::tcp::iostream stream;
};

#endif

