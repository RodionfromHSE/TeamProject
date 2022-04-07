#include "server.h"

Server::Server(uint16_t port, uint16_t limit) : ServerInterface(port), _limit(limit) {
    run();
}

bool Server::on_client(std::shared_ptr<net::Connection<EVENT>> client) {
    if (_objects.size() > _limit) {
        std::cerr << "Overflow!\n";
        return false;
    }
    return true;
}

const std::vector<Object> &Server::get_objects() {
    return _objects;
}

int cnt = 0;
void Server::handle_message(std::shared_ptr<net::Connection<EVENT>> client, net::Message<EVENT> &msg) {
    switch (msg.header.id) {
        case EVENT::SYNCHRONIZATION:
            int id;
            msg >> id;
            m_synHandler.add_message(msg, id);
            break;
        case EVENT::NOTHING:
            Point p;
            msg >> p;
            std::cout << p.x << ' ' << p.y << '\n';
            msg << ++cnt;
            send_to_client(client, msg);
            break;
        default:
            break;
    }
}

net::SynchronizedHandler<EVENT> &Server::syn_handler() {
    return m_synHandler;
}