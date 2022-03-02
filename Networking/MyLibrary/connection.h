#ifndef TEAMPROJECT_CONNECTION_H
#define TEAMPROJECT_CONNECTION_H

#include "fwd.h"
#include "message.h"
#include "tsqueue.h"

namespace myLibrary {

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
                                                         _ioContext(ioContext){

        }

        bool is_connected() {
            return false;
        }

        bool connect_to_server() {
            return false;
        }

        bool disconnect() {
            return false;
        }

        void send(const Message<T> &msg) {
        }

    private:
        Owner _owner;

        TSQueue<Message<T>> _queueOut;
        TSQueue<OwnedMessage<T>> &_queueIn;

        tcp::socket _socket;
        boost::asio::io_context &_ioContext;
    };
}

#endif //TEAMPROJECT_CONNECTION_H
