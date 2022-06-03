#include "server.h"

Server::Server(uint16_t port) : ServerInterface(port) {
    run();
}

bool Server::on_client(std::shared_ptr<net::Connection<EVENT>> client) {
    return true;
}

//const std::vector<Object> &Server::get_objects() {
//    return _objects;
//}

int cnt = 0;
void Server::handle_message(std::shared_ptr<net::Connection<EVENT>> client, net::Message<EVENT> &msg) {
    m_msgQueue.push_back(msg);
}

net::SynchronizedHandler<EVENT> &Server::syn_handler() {
    return m_synHandler;
}
