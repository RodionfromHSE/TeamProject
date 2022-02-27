#ifndef TEAMPROJECT_SERVER_H
#define TEAMPROJECT_SERVER_H


#include "fwd.h"
#include "connection.h"
#include "message.h"
#include "tsqueue.h"


using boost::asio::ip::tcp;
namespace myLibrary {

    template<typename T>
    struct ServerInterface {
        explicit ServerInterface(uint16_t port) : _acceptor(port, tcp::endpoint(tcp::v4(), port)), _idCount(0) {

        }

        ~ServerInterface() {
            stop();
        }

        bool run() {
            try {
                start_accept();

                _contextThr([&]() { _ioContext.run(); });
            } catch (std::exception &e) {
                std::cout << e.what() << '\n';
                return false;
            }
            std::cout << "Server runs!\n";
            return true;
        }

        void stop() {
            _ioContext.stop();

            if (_contextThr.joinable()) {
                _contextThr.join();
            }

            std::cout << "Server closed!\n";
        }

        void send_to_client(std::shared_ptr<Connection<T>> client, Message<T> &msg) {
            if (client && client->is_connected()) {
                client->send(msg);
                return;
            }

            disconnect_client(client);
            client.reset();
            _connections.erase(std::remove(_connections.begin(), _connections.end(), client), _connections.end());
        }

        void send_to_everyone(Message<T> &msg, std::shared_ptr<Connection<T>> ignoreClient) {
            bool isAnyDisconnected = false;

            for (auto &client: _connections) {
                if (client && client->is_connected()) {
                    if (client != ignoreClient) {
                        client->send(msg);
                    }
                    continue;
                }

                disconnect_client(client);
                client.reset();
                isAnyDisconnected = true;
            }

            if (isAnyDisconnected) {
                _connections.erase(std::remove(_connections.begin(), _connections.end(), nullptr), _connections.end());
            }
        }

        void update(std::size_t maxCount = -1) {
            std::size_t count = 0;

            while (count < maxCount && !_inMessages.empty()) {
                auto msg = _inMessages.back();
                _inMessages.pop_back();

                handle_message(msg);
            }
        }

    protected:
        void start_accept() {
            _acceptor.async_accept([&](boost::system::error_code ec, tcp::socket) {
                if (!ec) {
                    std::shared_ptr<Connection<T>> conn = std::make_shared<Connection<T>>(Connection<T>::Owner::SERVER,
                                                                                          _inMessages,
                                                                                          std::move(socket),
                                                                                          _ioContext);

                    if (conn) {
                        _connections.push_back(conn);
                    } else {
                        std::cout << "Rejected!\n";
                    }
                } else {
                    std::cout << "Server error!\n";
                }

                start_accept();
            });
        }

        bool connect_client(std::shared_ptr<Connection<T>> client) {
            return false;
        }

        void handle_message(std::shared_ptr<Connection<T>> client, Message<T> &msg) {

        }

        void disconnect_client(std::shared_ptr<Connection<T>> client) {

        }

    private:
        boost::asio::io_context _ioContext;
        std::thread _contextThr;
        tcp::acceptor _acceptor;

        std::vector<std::shared_ptr<Connection<T>>> _connections;
        std::vector<OwnedMessage<T>> _inMessages;

        uint16_t _idCount;
    };
}
#endif //TEAMPROJECT_SERVER_H
