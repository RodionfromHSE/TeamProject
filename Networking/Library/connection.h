#pragma once

#include "fwd.h"

using boost::asio::ip::tcp;
namespace net {

    template<typename T>
    struct Connection : std::enable_shared_from_this<Connection<T>> {
        enum class Owner {
            CLIENT,
            SERVER
        };

        Connection() = default;

        ~Connection() = default;


        Connection(Owner owner, TSQueue<OwnedMessage<T>> &queueIn, tcp::socket socket,
                   boost::asio::io_context &ioContext) : _owner(owner), _queueIn(queueIn), _socket(std::move(socket)),
                                                         _ioContext(ioContext) {
        }

        bool is_connected() {
            return _socket.is_open();
        }

        void disconnect() {
            if (is_connected()) {
                boost::asio::post(_ioContext, [this]() {
                    _socket.close();
                });
            }
        }

        void connect_to_server(const boost::asio::ip::tcp::resolver::results_type &endpoints) {
            if (_owner == Owner::CLIENT) {
                boost::asio::async_connect(_socket, endpoints,
                                           [this](boost::system::error_code ec,
                                                  const boost::asio::ip::tcp::endpoint& endpoint) {
                                               if (!ec) { read_header(); }
                                           });
            }
        }

        void connect_to_client(uint32_t id) {
            if (_owner == Owner::SERVER) {
                if (_socket.is_open()) {
                    _uid = id;
                    read_header();
                }
            }
        }

        void send(const Message<T> &msg) {
            boost::asio::post(_ioContext, [this, msg]() {
                // Hope you enjoy my variable aliases
                bool isOutQueueEmpty = _queueOut.empty();
                _queueOut.push_back(msg);
                if (isOutQueueEmpty) {
                    write_header();
                }
            });
        }


        [[nodiscard]] int get_id() const {
            return _uid;
        }

    protected:
        // MESSAGE
        void read_header() {
            boost::asio::async_read(_socket, boost::asio::buffer(&_inMessage.header, sizeof(Header<T>)),
                                    [&](boost::system::error_code ec, [[maybe_unused]] std::size_t bytes_transferred) {
                                        if (!ec) {
                                            if (_inMessage.header.size > 0) {
                                                _inMessage.body.resize(_inMessage.header.size);
                                                read_body();
                                            } else {
                                                add_to_incoming();
                                            }
                                        } else {
                                            std::cout << "Header reading failed: " << ec.message() << "\n";
                                            _socket.close();
                                        }
                                    });
        }

        void read_body() {
            boost::asio::async_read(_socket, boost::asio::buffer(_inMessage.body.data(), _inMessage.header.size),
                                    [&](boost::system::error_code ec, [[maybe_unused]] std::size_t bytes_transferred) {
                                        if (!ec) {
                                            assert(_inMessage.header.size == _inMessage.size());
                                            add_to_incoming();
                                        } else {
                                            std::cout << "Body reading failed\n";
                                            _socket.close();
                                        }
                                    });
        }

        void write_header() {
            boost::asio::async_write(_socket, boost::asio::buffer(&_queueOut.front().header, sizeof(Header<T>)),
                                     [&](boost::system::error_code ec, std::size_t bytes_transferred) {
                                         if (!ec) {
                                             if (_queueOut.front().header.size > 0) {
                                                 write_body();
                                             } else {
                                                 _queueOut.pop_front();
                                                 if (!_queueOut.empty()) {
                                                     write_header();
                                                 }
                                             }
                                         } else {
                                             std::cout << "Header writing failed\n";
                                             _socket.close();
                                         }
                                     });
        }

        void write_body() {
            boost::asio::async_write(_socket,
                                     boost::asio::buffer(_queueOut.front().body.data(), _queueOut.front().body.size()),
                                     [&](boost::system::error_code ec, std::size_t bytes_transferred) {
                                         if (!ec) {
                                             _queueOut.pop_front();
                                             if (!_queueOut.empty()) {
                                                 write_header();
                                             }
                                         } else {
                                             std::cout << "Body writing failed\n";
                                             _socket.close();
                                         }
                                     });
        }

        void add_to_incoming() {
            if (_owner == Owner::SERVER) {
                _queueIn.push_back({this->shared_from_this(), _inMessage});
            } else {
                _queueIn.push_back({nullptr, _inMessage});
            }
            read_header();
        }

    private:
        Owner _owner;
        uint32_t _uid = 0;

        TSQueue<Message<T>> _queueOut;
        TSQueue<OwnedMessage<T>> &_queueIn;

        tcp::socket _socket;
        boost::asio::io_context &_ioContext;

        Message<T> _inMessage;
    };
}
