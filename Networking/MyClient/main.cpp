#include <boost/asio.hpp>
#include <thread>
#include <iostream>
#include <string>
#include <vector>

using boost::asio::ip::tcp;

void checkConnection(boost::system::error_code &ec) {
    if (!ec) {
        std::cout << "Well done!\n";
        return;
    }
    std::cout << "Error: " << ec.message() << '\n';
}

//std::vector<char> &getBuffer{
//        inline static  = {};
//        return buf;
//};


std::vector<char> buf(1024);

void grabData(tcp::socket &socket) {
    socket.read_some(boost::asio::buffer(buf.data(), buf.size()),
                     [&](std::size_t len, boost::system::error_code &ec)
    {
        if (!ec) {
            std::cout << "\n\n+++" << len << "+++\n\n";
            for (auto c: buf) {
                std::cout << c;
            }
            grabData(socket);
        }
    }
}


int main(int argc, char *argv[]) {
    boost::asio::io_context ioContext;
    boost::system::error_code ec;
    tcp::endpoint endpoint(boost::asio::ip::make_address("91.198.174.192", ec), 443);

    std::thread thr([&]() { ioContext.run(); });



    tcp::socket socket(ioContext);
    socket.connect(endpoint, ec);

    checkConnection(ec);
    if (socket.is_open()) {
        grabData(socket);

        std::string request = "GET /index.html HTTP/1.1\r\n"
                              "Host: ru.wikipedia.org/wiki/\r\n"
                              "Connection: close\r\n\r\n";
        socket.write_some(boost::asio::buffer(request.data(), request.size()), ec);
    }
}
