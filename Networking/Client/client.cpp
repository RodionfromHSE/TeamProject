#include "client.h"

std::vector<Point> moves{{-1,  0},
                         {0, 1},
                         {0,  -1},
                         {1,  0}};

Point Client::move(int step) {
    return moves[step];
}

void Client::handle_message(net::Message<EVENT> msg, std::shared_ptr<net::Connection<EVENT>> sender_ptr) {
    switch (msg.header.id) {
        case EVENT::SYNCHRONIZATION:
            int id;
            msg >> id;
            m_synHandler.add_message(msg, id);
            break;
        case EVENT::NOTHING:
            int cnt;
            msg >> cnt;
            std::cout << "Message " << cnt << '\n';
        default:
            break;
    }
}


net::SynchronizedHandler<EVENT> &Client::syn_handler() {
    return m_synHandler;
}
