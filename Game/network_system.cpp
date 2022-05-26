#include "network_system.h"

ClientSystem::ClientSystem(uint16_t port) : m_clientPtr(new Client()), m_player(m_clientPtr, PLAYER_ID) {
    m_clientPtr->connect(host, PORT);
    m_player.set({0, 0});

    thread = std::thread([this]() {
        while (true) {
            update();
        }
    });
}

void ClientSystem::update() {
    // Here we'll new players arrive and old update
    m_clientPtr->update();

    if (m_player.isUpdatable())
        std::cout << m_player.get().x << ' ' << m_player.get().y << '\n';

}

