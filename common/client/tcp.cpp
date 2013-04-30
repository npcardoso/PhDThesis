#include "client/tcp.h"

#include "utils/debug.h"
#include "utils/time.h"

t_tcp_client::t_tcp_client(std::string host,
                       std::string port) {
  this->host = host;
  this->port = port;
  stream.connect(host, port);
}

t_tcp_client::~t_tcp_client() {
  stream.close();
}
void t_tcp_client::write(const std::string & s) {
  do {
    setup();
    stream << s;
  } while(!stream.good());
}


void t_tcp_client::setup(){
  while(!stream.good()) {
    stream.clear();
    stream.connect(host, port);
    if (!stream) {
      std::cout << "Error: " << stream.error().message() << std::endl;
      msleep(1000);
    }
  }
}
