#ifndef __TCP_SERVER_H__
#define __TCP_SERVER_H__

#include "service.h"

#include <boost/asio.hpp>

void tcp_server(boost::asio::io_service& io_service, 
                unsigned short port, 
                t_service & srv);


#endif
