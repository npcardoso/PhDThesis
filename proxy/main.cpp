#include "tcp_server.h"

#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

int main()
{
  try
  {
    boost::asio::io_service io_service;
    t_echo_service echo;
    t_threaded_service threaded_echo(t_service::t_ptr(new t_echo_service()));
    tcp_server(io_service, 1234, threaded_echo);
    io_service.run();
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
