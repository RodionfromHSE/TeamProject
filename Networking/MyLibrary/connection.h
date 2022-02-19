#ifndef TEAMPROJECT_CONNECTION_H
#define TEAMPROJECT_CONNECTION_H

#include "fwd.h"

namespace myServer{
    using boost::asio::ip::tcp;
    struct TCPConnection : std::enable_shared_from_this<TCPConnection> {
        using ptr_t = std::shared_ptr<TCPConnection>;

        static ptr_t create_ptr(boost::asio::io_context &io_context);

        tcp::socket& socket();

        void Start();
    private:
        explicit TCPConnection(boost::asio::io_context &io_context);
    private:
        tcp::socket _socket;
        std::string _msg{"Hi, Client!"};
    };
}

#endif //TEAMPROJECT_CONNECTION_H
