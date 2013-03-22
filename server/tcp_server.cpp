#include "tcp_server.h"

#include "utils/debug.h"

#include <string>

using boost::asio::ip::tcp;

void tcp_server(boost::asio::io_service& io_service, 
                unsigned short port, 
                t_service & srv) {
  tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), port));
  while (true) {
    tcp::iostream * stream = new tcp::iostream();
    acceptor.accept(*stream->rdbuf());
    debug("%s", "Got connection");
    if (*stream) {
      t_iostream_ptr tmp(stream);
      srv(tmp, tmp);
    }
  }
}

