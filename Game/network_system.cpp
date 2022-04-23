#include "network_system.h"

NetworkSystem::NetworkSystem(uint16_t port) : m_serverPtr(new Server(port)) {
}

void NetworkSystem::update() {
    // Here we'll new players arrive and old update
    m_serverPtr->update();

    // Arriving newbies
    auto newbies = m_serverPtr->syn_handler().get_newbies();
    for (auto nb : newbies){
        m_players.emplace_back(m_serverPtr, nb);
    }

    for (auto &player : m_players) {
        if (player.isUpdatable())
            std::cout << player.get().x << ' ' << player.get().y << '\n';
    }
}

