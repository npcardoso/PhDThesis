#include "tcp.h"

#include "utils/debug.h"


#include <string>

using boost::asio::ip::tcp;

void tcp_server (boost::asio::io_service & io_service,
                 unsigned short port,
                 t_service & srv) {
    tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), port));


    while (true) {
        t_ptr<tcp::iostream> stream(new tcp::iostream());
        acceptor.accept(*stream->rdbuf());
        debug("%s", "Got connection");

        if (*stream)
            srv(*stream, *stream);
    }
}

void tcp_client (std::string host,
                 unsigned short port,
                 t_service & srv) {
    t_ptr<tcp::iostream> stream(new tcp::iostream());
    stream->connect(host, std::to_string(port));

    if (*stream)
        srv(stream, stream);

    stream->close();
}