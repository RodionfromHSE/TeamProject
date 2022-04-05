#pragma once

#include "fwd.h"
#include "../Server/server.h"
#include "../Client/client.h"

namespace net {
    template<typename T, bool OnServer = true>
    struct Synchronized {
        Synchronized(std::shared_ptr<Server> server_ptr, int id) : m_server_ptr(std::move(server_ptr)),
                                                                   m_id(id) {
        }

        void set(const T &val) {
            m_value = val;

            Message<EVENT> msg;
            msg.header.id = EVENT::SYNCHRONIZATION;
            msg << m_value << m_id;

            // we want our value to the most actual
            m_server_ptr->syn_handler().clear(m_id);
            m_server_ptr->send_to_everyone(msg);
        }

        const T &get() {
            if (!m_server_ptr->syn_handler().empty(m_id)) {
                auto msg = m_server_ptr->syn_handler().last_message(m_id);
                assert(msg.header.size == sizeof m_value);
                msg >> m_value;
            }
            return m_value;
        }

    private:
        std::shared_ptr<Server> m_server_ptr;
        int m_id;
        T m_value;
    };

    template<typename T>
    struct Synchronized<T, false> {
        Synchronized(std::shared_ptr<Client> client_ptr, int id) : m_client_ptr(std::move(client_ptr)),
                                                                   m_id(id) {
        }


        void set(const T &val) {
            m_value = val;

            Message<EVENT> msg;
            msg.header.id = EVENT::SYNCHRONIZATION;
            msg << m_value << m_id;

            m_client_ptr->syn_handler().clear(m_id);
            m_client_ptr->send(msg);
        }

        [[nodiscard]] const T &get() {
            if (!m_client_ptr->syn_handler().empty(m_id)) {
                auto msg = m_client_ptr->syn_handler().last_message(m_id);
                assert(msg.header.size == sizeof m_value);
                msg >> m_value;
            }
            return m_value;
        }


    private:
        std::shared_ptr<Client> m_client_ptr;
        int m_id;
        T m_value;
    };
}
