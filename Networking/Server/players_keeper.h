#pragma once

#include "fwd.h"
#include "synchronizied.h"

struct PlayerKeeper {
    PlayerKeeper(std::shared_ptr<Server> serverPtr, net::TSQueue<net::Message<EVENT>> &messages) : m_serverPtr(
            std::move(serverPtr)), m_messages(messages) {}

    void update() {
        while (!m_messages.empty()) {
            auto msg = m_messages.pop_front();
            switch (msg.header.id) {
                case EVENT::SYNCHRONIZATION: {
                    int id;
                    msg >> id;
                    m_serverPtr->get_synHandler().add_message(msg, id);
                    msg << id;
                    m_serverPtr->send_to_everyone(msg);
                }
                    break;
                case EVENT::NEW_PLAYER: {
                    int id;
                    msg >> id;
                    m_players.emplace_back(m_serverPtr, id);
                }
                    break;
                default:
                    break;
            }
        }
        for (auto &player: m_players) {
            if (player.isUpdatable())
                std::cout << "Coordinates [" << player.getId() << "]: " << player.get().x << ' ' << player.get().y
                          << '\n';
        }
    }

private:
    std::shared_ptr<Server> m_serverPtr;
    net::TSQueue<net::Message<EVENT>> &m_messages;
    std::vector<net::Synchronized<Point, Usage::OnServer>> m_players;
};