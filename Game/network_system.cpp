#include "network_system.h"

NetworkSystem::NetworkSystem(uint16_t port) : m_serverPtr(new Server(port)) {
}

void NetworkSystem::update() {
    // Here we'll new players arrive and old update
    m_serverPtr->update();
    for (auto &player : m_players) {
        // do_smth();
    }
}

