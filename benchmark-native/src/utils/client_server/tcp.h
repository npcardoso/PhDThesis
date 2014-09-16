#ifndef __TCP_H_2f9b522b183a87f7ca350bd5671066a43cd7de68__
#define __TCP_H_2f9b522b183a87f7ca350bd5671066a43cd7de68__

#include "service.h"

#include <boost/asio.hpp>

void tcp_server (boost::asio::io_service & io_service,
                 unsigned short port,
                 t_service & srv);

#endif