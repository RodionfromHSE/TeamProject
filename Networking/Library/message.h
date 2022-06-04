#pragma once

#include "fwd.h"

namespace net {

    template<typename T>
    struct Header{
        T id{};
        uint32_t size = 0;
    };

    template<typename T>
    struct Message{
        [[nodiscard]] std::size_t size() const noexcept {
            return body.size();
        }

        void clear() {
            header.size = 0;
            body.resize(header.size);
        }

        // Nice output
        friend std::ostream & operator<<(std::ostream& os, Message& msg){
            os << "ID:" << int(msg.header.id) << " Size:" << msg.header.size;
            return os;
        }

        // Message recording
        template<typename DataType>
        friend Message& operator<<(Message<T> &msg, const DataType& data){
            static_assert(std::is_standard_layout_v<DataType>);

            std::size_t sz = msg.size();

            msg.body.resize(sz + sizeof(DataType));

            std::memcpy(msg.body.data() + sz, reinterpret_cast<const uint8_t*>(&data), sizeof(DataType));

            msg.header.size = msg.size();

            return msg;
        }

        // Message transfer
        template<typename DataType>
        friend Message& operator>>(Message<T>& msg, DataType& data){
            static_assert(std::is_standard_layout_v<DataType>);

            std::size_t sz = msg.size() - sizeof(DataType);
            std::memcpy(reinterpret_cast<uint8_t*>(&data), msg.body.data() + sz, sizeof(DataType));

            msg.body.resize(sz);
            msg.header.size = msg.size();

            return msg;
        }
    public:
        std::vector<uint8_t> body;
        Header<T> header;
    };

    template<typename T>
    struct Connection;

    template<typename T>
    struct OwnedMessage{
        friend std::ostream &operator<<(std::ostream& os, OwnedMessage ownMsg){
            return os << ownMsg.msg;
        }
    public:
        std::shared_ptr<Connection<T>> remote;
        Message<T> msg;
    };
}
