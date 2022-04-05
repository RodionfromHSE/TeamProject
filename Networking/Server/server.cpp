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

void Server::handle_message(std::shared_ptr<net::Connection<EVENT>> client, net::Message<EVENT> &msg) {
    Point coors;
    PLAYER_STATE state;

    msg >> state >> coors;
    std::cout << coors.x << ' ' << coors.y << " got coordinates\n";
    if (__COUNTER__ % 2)
        msg << coors << PLAYER_STATE::JUMP;
    else
        msg << coors << PLAYER_STATE::RUN;

    send_to_client(client, msg);
}

net::SynchroniziedHandler<EVENT> &Server::syn_handler() {
    return m_synHandler;
}
