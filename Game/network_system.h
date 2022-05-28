#pragma once

#include "fwd.h"
#include "system.h"
#include "../Networking/Client/client.h"
#include "../Networking/Server/server.h"
#include "../Networking/Library/synchronizied.h"

namespace {
    const std::string HOST = "127.0.0.1";
    const uint16_t PORT = 60'000;
    const int ID = 770;
}

struct PlayerCoordinates {
    int x = 0, y = 0;
};


struct NetworkSystem : System {
    // TODO: Synchronize Player
    NetworkSystem(std::string host = HOST, uint16_t port = PORT);

    // Hmm... update something
    void update() override;

    inline void set(PlayerCoordinates pc){
        m_player.set(pc);
    }

    inline ~NetworkSystem() {
        if (thread.joinable()) { thread.join(); }
    }

private:
    std::shared_ptr<Client> m_clientPtr;
    net::Synchronized<PlayerCoordinates, Usage::OnClient> m_player;
    std::thread thread;
};



