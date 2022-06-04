#pragma once


#include "client_interface.h"
#include "object.h"
#include "game_enum.h"
#include "message.h"
#include "synchronizied_handler.h"


struct Client : net::ClientInterface<EVENT> {
    void handle_message(net::Message<EVENT> msg, std::shared_ptr<net::Connection<EVENT>> sender_ptr) override;

    Point move(int step);

    net::SynchronizedHandler<EVENT> &syn_handler();

private:
    net::SynchronizedHandler<EVENT> m_synHandler;
};
