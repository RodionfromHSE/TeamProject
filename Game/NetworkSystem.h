#pragma once

#include "fwd.h"
#include "system.h"
#include "../Networking/Client/client.h"
#include "../Networking/Library/synchronizied.h"

struct PlayerType;

struct NetworkSystem : System {
    // TODO: Synchronize Player
    NetworkSystem(PlayerType& Player);
    // TODO: For client - not our case
    bool connect(std::string &host);
    // Hmm... update something
    void update() override;

    void make_synch
private:
    std::shared_ptr<Client> m_clientPtr;
    std::vector<net::Synchronized<PlayerType>> m_players;
};

