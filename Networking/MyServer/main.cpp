#include <boost/asio.hpp>
//#include <thread>
#include <iostream>
#include <string>


void checkConnection(boost::system::error_code& ec) {
    if (!ec){
        std::cout << "Well done!\n";
        return;
    }
    std::cout << "Error: " << ec.message() << '\n';
}


using boost::asio::ip::tcp;

int main(int argc, char* argv[]) {
    boost::asio::io_context ioContext;
    boost::system::error_code ec;
    tcp::endpoint endpoint(boost::asio::ip::make_address("91.198.174.192", ec), 443);

    tcp::socket socket(ioContext);
    socket.connect(endpoint, ec);

    checkConnection(ec);
    if (socket.is_open()){
        std::string request = "GET /index.html HTTP/1.1\r\n"
                              "Host: ru.wikipedia.org/wiki/\r\n"
                              "Connection: close\r\n\r\n";
        socket.write_some(boost::asio::buffer(request.data(), request.size()), ec);
        checkConnection(ec);
        std::size_t bytes = socket.available();
        std::cout << bytes << " bytes got!\n";

//        if (bytes > 0){
//            std::vector<char> msg(bytes);
//            socket.read_some(boost::asio::buffer(msg.data(), msg.size()), ec);
//
//            for (auto c : msg) {
//                std::cout << c;
//            }
//        }


    }
}
