#ifndef CLIENT_H
#define CLIENT_H

#include "connection.h"
#include "client.h"
#include "fwd.h"
#include "message.h"

using boost::asio::ip::tcp;
namespace myLibrary {
    template<typename T>
    struct ClientInterface {
        ClientInterface() : _socket(_ioContext){

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
                auto endpoint(resolver.resolve(host, std::to_string(port)));

                _connection = std::make_unique<Connection<T>>(Connection<T>::Owner::CLIENT, _inQueue,
                                                              boost::asio::ip::tcp::socket(_ioContext), _ioContext);
                _connection->connect_to_server(endpoint);

                _thrClient = std::thread([&]() { _ioContext.run(); });
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
        }

    public:
        TSQueue<OwnedMessage<T>> _inQueue;
    protected:
        void send(Message<T> &msg) {
            _connection->send(msg);
        }


        // It's needed in order to create "premature" contact between server and client
        boost::asio::io_context _ioContext;
        tcp::socket _socket;
        std::thread _thrClient;
    private:

        std::unique_ptr<Connection < T>> _connection;
    };
}
#endif //CLIENT_H
