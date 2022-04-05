#pragma once

#include <vector>
#include <iostream>
#include "server_interface.h"
#include "game_enum.h"
#include "message.h"
#include "object.h"
#include "synchronizied_handler.h"

/*
 * Goal it to create a server supporting vector of objects. So customer will be
 * able to iterate through synchronized objects and change them.
 */
struct Server : net::ServerInterface<EVENT>{
    explicit Server(uint16_t port, uint16_t limit = 100);

    bool on_client(std::shared_ptr<net::Connection<EVENT>> client) override;

    void handle_message(std::shared_ptr<net::Connection<EVENT>> client, net::Message<EVENT> &msg) override;

    const std::vector<Object>& get_objects();

    net::SynchronizedHandler<EVENT> &syn_handler();

private:
    net::SynchronizedHandler<EVENT> m_synHandler;
    std::vector<Object> _objects;
    uint16_t _limit;
};
