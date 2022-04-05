#pragma once

#include <vector>
#include <iostream>
#include "client_interface.h"
#include "object.h"
#include "game_enum.h"
#include "message.h"
#include "synchronizied_handler.h"


struct Client : net::ClientInterface<EVENT> {
    Client(Object player);

    void handle_message(net::Message<EVENT> msg, std::shared_ptr<net::Connection<EVENT>> sender_ptr) override;

    void move(int step);

    net::SynchroniziedHandler<EVENT> &syn_handler();

private:
    net::SynchroniziedHandler<EVENT> m_synHandler;
    Object m_player;
};
