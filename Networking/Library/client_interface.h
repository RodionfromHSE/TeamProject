#ifndef CLIENT_H
#define CLIENT_H

#include "connection.h"
#include "client_interface.h"
#include "fwd.h"
#include "message.h"

using boost::asio::ip::tcp;
namespace net {
    template<typename T>
    struct ClientInterface {
        ClientInterface() : _socket(_ioContext) {

        };

        ~ClientInterface() {
            //  TODO: is such destructor is memory leak here? Further more in heirs?
            disconnect();
        };

        [[nodiscard]] bool is_connected() const noexcept {
            if (_connection) {
                return _connection->is_connected();
            }
            return false;
        }

        bool connect(const std::string &host, uint16_t port) {
            try {
                tcp::resolver resolver(_ioContext);
                boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(host, std::to_string(port));

                _connection = std::make_unique<Connection<T>>(Connection<T>::Owner::CLIENT, inQueue,
                                                              boost::asio::ip::tcp::socket(_ioContext), _ioContext);
                _connection->connect_to_server(endpoints);

                _thrClient = std::thread([this]() { _ioContext.run(); });
            } catch (std::exception &e) {
                std::cout << "Failed to connect with error:" << e.what() << std::endl;
                return false;
            }
            return true;
        }

        void disconnect() {
            if (is_connected()) {
                _connection->disconnect();
            }

            _ioContext.stop();

            if (_thrClient.joinable())
                _thrClient.join();

            _connection.release();
        }

        void send(Message <T> &msg) {
            if (is_connected())
                _connection->send(msg);
        }

        virtual void handle_message(Message<T> msg, std::shared_ptr<Connection<T>> sender_ptr) = 0;

        void update() {
            inQueue.wait();
            while (!inQueue.empty()){
                // Here we know where message from
                auto ownMsg = inQueue.pop_front();
                handle_message(std::move(ownMsg.msg), ownMsg.remote);
            }
        }

    public:
        TSQueue <OwnedMessage<T>> inQueue;
    protected:


        // It's needed in order to create "premature" contact between server and client
        boost::asio::io_context _ioContext;
        tcp::socket _socket;
        std::thread _thrClient;
    private:

        std::unique_ptr<Connection < T>> _connection;
    };
}
#endif //CLIENT_H
