#include "serialization/unserializers/json.h"
#include "serialization/serializers/json.h"

#include "client/tcp.h"
#include "server/servers/tcp_server.h"
#include "server/services/threaded.h"
#include "server/services/json.h"

class t_json_process: public t_json_service {
public:
  virtual void operator ()(std::istream & in,
                           std::ostream & out,
                           const boost::property_tree::ptree & pt){
    t_observation_sink::t_ptr sink(new t_json_observation_serializer(out, t_element_group_writer::t_ptr(new t_json_array())));
    t_json_observation_unserializer unserializer(sink);
    unserializer(pt);
  }
};

class t_proxy_service: public t_service {
public:
  t_proxy_service(std::string host,
                  std::string port,
                  t_service::t_ptr srv): client(host, port), srv(srv) {
  }
  virtual void operator ()(std::istream & in,
                           std::ostream & out) {
    std::stringstream buf;
    (*srv)(in, buf);
    client.write(buf.str());
  }
private:
  t_tcp_client client;
  t_service::t_ptr srv;
};


int main(int argc, char **argv) {
  std::string host="127.0.0.1";
  std::string dst_port="1235";
  int src_port=1234;

  try {
    boost::asio::io_service io_service;
    
    t_json_service::t_ptr json_service(new t_json_process());
    t_service::t_ptr service(new t_json_adapter_service(json_service));
    t_service::t_ptr proxy(new t_proxy_service(host, dst_port, service));

    tcp_server(io_service, src_port, *proxy);
    io_service.run();
  }
  catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
