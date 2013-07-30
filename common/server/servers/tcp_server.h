#ifndef __SERVER_SERVERS_TCP_SERVER_H__
#define __SERVER_SERVERS_TCP_SERVER_H__

#include "server/service.h"

#include <boost/asio.hpp>

void tcp_server (boost::asio::io_service & io_service,
                 unsigned short port,
                 t_service & srv);


#endif