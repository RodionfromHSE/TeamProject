#pragma once

#include "fwd.h"
#include "system.h"
#include "../Networking/Client/client.h"
#include "../Networking/Server/server.h"
#include "../Networking/Library/synchronizied.h"

struct PlayerCoordinates {
    int x = 0, y = 0;
};


struct NetworkSystem : System {
    // TODO: Synchronize Player
    NetworkSystem(uint16_t port);

    // Hmm... update something
    void update() override;

private:
    std::shared_ptr<Server> m_serverPtr;
    std::vector<net::Synchronized<PlayerCoordinates>> m_players;
};

