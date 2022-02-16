#include <boost/asio.hpp>
#include <thread>
#include <iostream>
#include <string>
#include "../MyLibrary/include/server.h"

using boost::asio::ip::tcp;

int main(int argc, char* argv[]) {
    try {
        myServer::TCPServer server(myServer::IPV::ipv4, 1337);
        server.Run();



    } catch (std::exception &e) {
        std::cout << e.what() << '\n';
    }
}
