#ifndef TEAMPROJECT_SERVER_H
#define TEAMPROJECT_SERVER_H

#include "connection.h"
#include <string>
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
namespace myServer{
    enum class IPV{
        ipv4
    };

    struct TCPServer{
        TCPServer(IPV ipv, int port);

        int Run();

    private:
        void startAccept();
    private:
        IPV _ipv;
        int _port;

        boost::asio::io_context _io_context;
        tcp::acceptor _acceptor;
    };
}
#endif //TEAMPROJECT_SERVER_H
