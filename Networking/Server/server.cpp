#include "server.h"

Server::Server(uint16_t port, uint16_t limit) : ServerInterface(port), _limit(limit) {
    run();
}

bool Server::on_client(std::shared_ptr<myLibrary::Connection<OBJECT_TYPE>> client) {
    if (_objects.size() > _limit) {
        std::cerr << "Overflow!\n";
        return false;
    }
    return true;
}

const std::vector<Object> &Server::get_objects() {
    return _objects;
}

void Server::handle_message(std::shared_ptr<myLibrary::Connection<OBJECT_TYPE>> client, myLibrary::Message<OBJECT_TYPE> &msg) {
    Point coors;
    PLAYER_STATE state;

    msg >> coors >> state;
    std::cout << coors.x << ' ' << coors.y << '\n';
    msg << coors << state;

    send_to_client(client, msg);
}
