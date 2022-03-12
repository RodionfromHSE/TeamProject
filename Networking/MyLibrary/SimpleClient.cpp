#include "connection.h"
#include "client.h"
#include "tsqueue.h"
#include "message.h"
#include "fwd.h"
#include <chrono>
#include <thread>

enum class SIGNAL {
    ServerPing
};

using namespace myLibrary;

struct Client : ClientInterface<SIGNAL> {
    void ping() {
        if (is_connected()) {
            Message<SIGNAL> msg;
            msg.header.id = SIGNAL::ServerPing;
            msg << "Message " << ++_msgCount;

            send(msg);
        }
    }

private:
    uint32_t _msgCount = 0;
};

int main() {
    try {
        Client c;

        // Just skip this
//        boost::asio::io_context io;
//        boost::asio::ip::tcp::socket s(io);
//        tcp::resolver resolver(io);
//        boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve("127.0.0.1", "600");
//        boost::asio::connect(s, endpoints);
//        if (s.is_open())
//            std::cout << "What???" << std::endl;
//        for(tcp::endpoint const& endpoint : endpoints)
//        {
//            std::cout << endpoint << "\n";
//        }


        c.connect("127.0.0.1", 60000);
        for (int i = 0; i < 5; ++i) {
            c.ping();
        }

        // a small delay
        {
            using namespace std::this_thread;     // sleep_for, sleep_until
            using namespace std::chrono_literals; // ns, us, ms, s, h, etc.
            using std::chrono::system_clock;

            sleep_for(1s);
        }
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
