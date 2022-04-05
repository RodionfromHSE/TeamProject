#include "client.h"

std::vector<Point> moves{{1,  0},
                         {-1, 0},
                         {0,  -1},
                         {0,  1}};

void Client::move(int step) {
    m_player.set(m_player.coordinates() + moves[step]);
    net::Message<EVENT> msg;
    msg.header.id = EVENT::NOTHING;
//    std::cout << _player.coordinates().x << ' ' << _player.coordinates().y << " after change\n";
    msg << m_player.coordinates() << m_player.state();
    send(msg);
}

Client::Client(Object player) : m_player(std::move(player)) {
}

void Client::handle_message(net::Message<EVENT> msg, std::shared_ptr<net::Connection<EVENT>> sender_ptr) {
    switch (msg.header.id) {
        case EVENT::SYNCHRONIZATION:
            int id;
            msg >> id;
            m_synHandler.add_message(msg, id);
            break;
        default:
            break;
    }
}

net::SynchroniziedHandler<EVENT> &Client::syn_handler() {
    return m_synHandler;
}
