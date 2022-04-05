#pragma once

#include "fwd.h"
#include "connection.h"

using boost::asio::ip::tcp;
namespace net {
    template<typename T>
    struct ClientInterface {
        ClientInterface() = default;

        ~ClientInterface() {
            //  TODO: is such destructor is memory leak here? Further more in heirs?
            disconnect();
        };

        void send(Message<T> &msg) {
            if (is_connected())
                m_connection->send(msg);
        }

        [[nodiscard]] bool is_connected() const noexcept {
            if (m_connection) {
                return m_connection->is_connected();
            }
            return false;
        }

        bool connect(const std::string &host, uint16_t port) {
            try {
                tcp::resolver resolver(m_ioContext);
                boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(host, std::to_string(port));

                m_connection = std::make_unique<Connection<T>>(Connection<T>::Owner::CLIENT, m_inQueue,
                                                               boost::asio::ip::tcp::socket(m_ioContext), m_ioContext);
                m_connection->connect_to_server(endpoints);

                m_thrClient = std::thread([this]() { m_ioContext.run(); });
            } catch (std::exception &e) {
                std::cout << "Failed to connect with error:" << e.what() << std::endl;
                return false;
            }
            return true;
        }

        void disconnect() {
            if (is_connected()) {
                m_connection->disconnect();
            }

            m_ioContext.stop();

            if (m_thrClient.joinable())
                m_thrClient.join();

            m_connection.release();
        }

        virtual void handle_message(Message<T> msg, std::shared_ptr<Connection<T>> sender_ptr) = 0;

        void update() {
            m_inQueue.wait();
            while (!m_inQueue.empty()) {
                // Here we know where message from
                auto ownMsg = m_inQueue.pop_front();
                handle_message(std::move(ownMsg.msg), ownMsg.remote);
            }
        }

    private:
        TSQueue<OwnedMessage<T>> m_inQueue;

        // It's needed in order to create "premature" contact between server and client
        boost::asio::io_context m_ioContext;
        std::thread m_thrClient;

        std::unique_ptr<Connection<T>> m_connection;
    };
}
