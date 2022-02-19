#include "server.h"

//namespace myServer {
//    TCPServer::TCPServer(IPV ipv, int port) : _ipv(ipv), _port(port),
//                                              _acceptor(_io_context, tcp::endpoint(tcp::v4(), _port)) {
//
//    }
//
//    int TCPServer::Run() {
//        try {
//            std::cout << '?' << '\n';
//            startAccept();
//            _io_context.run();
//        } catch (std::exception &e) {
//            std::cout << e.what() << '\n';
//            return -1;
//        }
//        return 0;
//    }
//
//    void TCPServer::startAccept() {
//        auto connection = TCPConnection::create_ptr(_io_context);
//
//        std::cout << "Happens before!\n";
//        _acceptor.async_accept(connection->socket(), [connection, this](const boost::system::error_code &err){
//            if (!err){
//                std::cout << '+' << '\n';
//                connection->Start();
//            }
//            std::cout << '-' << '\n';
//
//            startAccept();
//        });
//    }
//}

