#pragma once

#include "fwd.h"
#include "system.h"
#include "../Networking/Client/client.h"
#include "../Networking/Server/server.h"
#include "../Networking/Library/synchronizied.h"

namespace constant {
    inline const std::string HOST = "127.0.0.1";
    inline const uint16_t PORT = 1234;
    inline int ID = 770;
}

//void login(){
//    std::cout << "Hello!\n"
//                 "What is your id? ";
//    std::cin >> constant::ID;
//}

struct PlayerCoordinates {
    int x = 0, y = 0;
};


struct NetworkSystem : System {
    // TODO: Synchronize Player
    NetworkSystem(int id = constant::ID, std::string host = constant::HOST, uint16_t port = constant::PORT);

    // Hmm... update something
    void update() override;

    inline void set(PlayerCoordinates pc){
        m_player.set(pc);
    }

    template<class T>
    std::shared_ptr<net::Synchronized<T, Usage::OnClient>> make_synchronized(int id){
        return std::make_shared<net::Synchronized<T, Usage::OnClient>>(m_clientPtr, id);
    }

    int getId(){
        return m_player.getId();
    }

    inline ~NetworkSystem() {
        if (thread.joinable()) { thread.detach(); }
    }

private:
    std::shared_ptr<Client> m_clientPtr;
    net::Synchronized<PlayerCoordinates, Usage::OnClient> m_player;
    std::thread thread;
};



