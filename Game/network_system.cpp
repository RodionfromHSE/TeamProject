#include "network_system.h"

NetworkSystem::NetworkSystem(std::string host, uint16_t port) : m_clientPtr(nullptr), m_player(m_clientPtr, ID){
    m_clientPtr->connect(host, port);
    if (!m_clientPtr->is_connected())
        throw std::logic_error("Can't establish connection:(");
    thread = std::thread([this](){
        while (true){
            update();
        }
    });
}

void NetworkSystem::update() {
    m_clientPtr->update();
}
