#include "server/servers/tcp_server.h"
#include "server/services/echo.h"
#include "server/services/threaded.h"
#include "server/services/json.h"

#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

int main() {
  try {
    boost::asio::io_service io_service;
    
    t_service::t_ptr service(new t_echo_service());
    (*service)(std::cin, std::cout);

    t_threaded_service threaded_service(service);
    tcp_server(io_service, 1234, threaded_service);
    io_service.run();
  }
  catch (std::exception& e) {
  //  std::cerr << e.what() << std::endl;
  }

  return 0;
}
