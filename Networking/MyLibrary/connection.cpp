#include "connection.h"

//namespace myServer {
//    void TCPConnection::Start() {
//        auto strong_this = shared_from_this();
//
//        boost::asio::async_write(_socket, boost::asio::buffer(_msg),
//                                 [strong_this](const boost::system::error_code &err, std::size_t bytes_transferred) {
//                                     if (err) {
//                                         std::cout << "Boom!\n";
//                                     } else {
//                                         std::cout << "Sent " << bytes_transferred << " bytes of data!";
//                                     }
//                                 });
//    }
//
//    TCPConnection::TCPConnection(boost::asio::io_context &io_context) : _socket(io_context) {
//
//    }
//
//    tcp::socket &TCPConnection::socket() {
//        return _socket;
//    }
//
//    TCPConnection::ptr_t TCPConnection::create_ptr(boost::asio::io_context &io_context) {
//        return ptr_t(new TCPConnection(io_context));
//    }
//
//}
