#ifndef TEAMPROJECT_SERVER_H
#define TEAMPROJECT_SERVER_H


#include "connection.h"
#include "fwd.h"
#include "message.h"
#include "tsqueue.h"


using boost::asio::ip::tcp;
namespace myLibrary {

    template<typename T>
    struct ServerInterface {
        explicit ServerInterface(uint16_t port) : _acceptor(_ioContext, tcp::endpoint(tcp::v4(), port)), _idCount(10'000) {
        }

        ~ServerInterface() {
            stop();
        }

        bool run() {
            try {
                start_accept();

                _contextThr = std::thread([this]() { _ioContext.run(); });
            } catch (std::exception &e) {
                std::cout << e.what() << std::endl;
                return false;
            }
            std::cout << "Server runs!" << std::endl;
            return true;
        }

        void stop() {
            _ioContext.stop();

            if (_contextThr.joinable()) {
                _contextThr.join();
            }

            std::cout << "Server closed!" << std::endl;
        }

        void send_to_client(std::shared_ptr<Connection<T>> client,
                            Message<T> &msg) {
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

            for (auto &client : _connections) {
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

            while (count++ < maxCount && !_inMessages.empty()) {
                auto msg = _inMessages.pop_back();

                handle_message(msg.remote, msg.msg);
            }

        }

    protected:
        void start_accept() {
            _acceptor.async_accept([this](const boost::system::error_code& ec, tcp::socket socket) {
                if (!ec) {
//                    std::shared_ptr<Connection<T>> conn = nullptr;
                    std::shared_ptr<Connection<T>> conn = std::make_shared<Connection<T>>(Connection<T>::Owner::SERVER,
                                                                                          _inMessages,
                                                                                          std::move(socket),
                                                                                          _ioContext);

                    if (conn) {
                        std::cout << "Got connection!" << std::endl;
                        _connections.push_back(conn);
                        _connections.back()->connect_to_client(_idCount++);
                    } else {
                        std::cout << "Rejected!" << std::endl;
                    }
                } else {
                    std::cout << "Server error!" << std::endl;
                }
                start_accept();
            });
        }

        bool connect_client(std::shared_ptr<Connection<T>> client) {
            return false;
        }

        void handle_message(std::shared_ptr<Connection<T>> client,
                            Message<T> &msg) {
        }

        void disconnect_client(std::shared_ptr<Connection<T>> client) {
        }

    private:
        boost::asio::io_context _ioContext;
        std::thread _contextThr;
        tcp::acceptor _acceptor;

        std::vector<std::shared_ptr<Connection<T>>> _connections;
        TSQueue<OwnedMessage<T>> _inMessages;

        uint16_t _idCount;
    };
}// namespace myLibrary
#endif//TEAMPROJECT_SERVER_H
