#ifndef CLIENT_H
#define CLIENT_H

#include "connection.h"
#include "fwd.h"
#include "message.h"

using boost::asio::ip::tcp;
namespace myLibrary {
    struct ClientInterface {
        ClientInterface() : _socket(_ioContext);

        ~ClientInterface() {
            if (is_connected()) {
                _connection->disconnect();
            }
        };

        bool is_connected() const noexcept {
            if (_connection) {
                return _connection->is_connected();
            }
            return false;
        }

        bool connect(const std::string &host, uint16_t port) {
            try {
                tcp::resolver resolver(_ioContext);
                auto endpoint(resolver.resolve(host, std::to_string(port)));

                _connection = std::make_unique<Connection<T>>(Owner::CLIENT, _inQueue,
                                                              boost::asio::socket(_ioContext), _ioContext);
                _connection->connect_to_server(endpoint);

                _thrClient = std::thread([&]() { _ioContext.run(); });
            } catch (std::exception &e) {
                std::cout << "Failed to connect with error:" << e.what() << '\n';
                return false;
            }
            return true;
        }

        void disconnect() {
            if (is_connected()) {
                _connection->disconnect();
            }
        }

    protected:
        void send(Message<T> &msg) {
            _connection->send(msg);
        }

    private:
        // It's needed in order to create "premature" contact between server and client
        boost::asio::io_context _ioContext;
        tcp::socket _socket;
        std::thread _thrClient;

        std::unique_ptr<TCPConnection < T>> _connection;

        TSQueue<OwnedMessage<T>> _inQueue;

    };
}
#endif //CLIENT_H
