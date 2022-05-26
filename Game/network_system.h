#pragma once

#include "fwd.h"
#include "system.h"
#include "../Networking/Client/client.h"
#include "../Networking/Server/server.h"
#include "../Networking/Library/synchronizied.h"
#include "../Networking/Library/synchronizied_handler.h"

struct PlayerCoordinates {
    int x = 0, y = 0;
};

const std::string host = "127.0.0.1";
const int PORT = 60'000;
const int PLAYER_ID = 770;

struct ClientSystem : System {
    // TODO: Synchronize Player
    ClientSystem(uint16_t port);

    // Hmm... update something
    void update() override;

    PlayerCoordinates getPlayer(){ return m_player.get(); }
    void setCoors(int x, int y){ m_player.set({x, y}); }

    inline ~ClientSystem(){
        if (thread.joinable()) { thread.join(); }
    }

private:
    std::shared_ptr<Client> m_clientPtr;
    net::Synchronized<PlayerCoordinates, Parameter::OnClient> m_player;
    std::thread thread;
};

