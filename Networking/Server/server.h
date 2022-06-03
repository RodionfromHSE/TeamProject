#pragma once

#include <vector>
#include <iostream>
#include "server_interface.h"
#include "game_enum.h"
#include "message.h"
#include "object.h"
#include "synchronizied_handler.h"

/*
 * Goal is to create a server supporting vector of objects. So customer will be
 * able to iterate through synchronized objects and change them.
 */
// TODO: garbage queue
struct Server : net::ServerInterface<EVENT> {
    explicit Server(uint16_t port);

    bool on_client(std::shared_ptr<net::Connection<EVENT>> client) override;

    void handle_message(std::shared_ptr<net::Connection<EVENT>> client, net::Message<EVENT> &msg) override;

    inline net::SynchronizedHandler<EVENT> &get_synHandler() {
        return m_synHandler;
    }
    inline net::TSQueue<net::Message<EVENT>> &get_msgQueue() {
        return m_msgQueue;
    }

    net::SynchronizedHandler<EVENT> &syn_handler();

private:
    net::SynchronizedHandler<EVENT> m_synHandler;
    net::TSQueue<net::Message<EVENT>> m_msgQueue;
};
