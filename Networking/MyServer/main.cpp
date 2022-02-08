#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include "Base.h"

using boost::asio::ip::tcp;

int main(int argc, char* argv[]) {
    try {
        boost::asio::io_context context;
        try {
            tcp::acceptor acceptor(context, tcp::endpoint(tcp::v4(), 1337));
        } catch (std::exception &e) {
            std::cout << e.what() << '\n';
            throw;
        }
        tcp::acceptor acceptor(context, tcp::endpoint(tcp::v4(), 1337));
        while (true) {
            tcp::socket socket(context);
            acceptor.accept(socket);

            std::string msg("Hello, Client!");
            boost::system::error_code err;

            boost::asio::write(socket, boost::asio::buffer(msg), err);
        }
    } catch (std::exception &e) {
        std::cout << e.what() << '\n';
    }
}
