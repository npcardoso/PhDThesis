#include "tcp_server.h"
#include "json.h"

#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

int main() {
  try {
    boost::asio::io_service io_service;
    t_echo_service echo;
    t_json_service::t_ptr json_service(new t_json_debug());
    t_service::t_ptr service(new t_json_parser_service(json_service));
    (*service)(std::cin, std::cout);

    t_threaded_service threaded_json(service);
    tcp_server(io_service, 1234, threaded_json);
    io_service.run();
  }
  catch (std::exception& e) {
  //  std::cerr << e.what() << std::endl;
  }

  return 0;
}
