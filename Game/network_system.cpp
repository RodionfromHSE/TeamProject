#include "network_system.h"

NetworkSystem::NetworkSystem(int id, std::string host, uint16_t port) : m_clientPtr(new Client()), m_player(m_clientPtr, id){
    m_clientPtr->connect(host, port);
    if (!m_clientPtr->is_connected())
        throw std::logic_error("Can't establish connection:(");
    m_player.initOnServer();

    thread = std::thread([this](){
        while (true){
            update();
        }
    });
}

void NetworkSystem::update() {
    m_clientPtr->update();
}
