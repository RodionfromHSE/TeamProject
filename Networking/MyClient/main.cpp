#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include "../MyLibrary/include/server.h"

using boost::asio::ip::tcp;

int main(int argc, char* argv[]) {
    try {
        boost::asio::io_context context;

        tcp::resolver resolver(context);
        auto endpoint = resolver.resolve("127.0.0.1", "1337");

        tcp::socket socket(context);
        boost::asio::connect(socket, endpoint);

        while (true) {
            // At the moment it's a good result I think:)
            std::vector<char> buf;
            buf.resize(100);
            boost::system::error_code err;

            auto len = socket.read_some(boost::asio::buffer(buf), err);
            if (err == boost::asio::error::eof){
                break;
            } else if (err) {
                throw boost::system::system_error(err);
            }

//            std::cout.write(buf.data(), len);
            for (auto &c : buf){
                std::cout << c;
            }
            std::cout << '\n';
        }
    } catch (std::exception &e) {
        std::cout << e.what() << '\n';
    }
}
