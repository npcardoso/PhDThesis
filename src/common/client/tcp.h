#ifndef __COMMON_CLIENT_TCP_H__
#define __COMMON_CLIENT_TCP_H__

#include "types.h"

#include "client.h"

#include <boost/asio.hpp>
#include <sstream>

class t_tcp_client : public t_client {
public:
    DEFINE_BOOST_SHARED_PTRS(t_tcp_client);

    t_tcp_client (std::string host,
                  std::string port);

    ~t_tcp_client ();

    virtual t_client & operator << (const std::string & s);
private:
    void setup ();
    std::string host, port;
    boost::asio::ip::tcp::iostream stream;
};

#endif