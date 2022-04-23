#include "client.h"
#include "synchronizied.h"

int main() {
    std::shared_ptr<Client> client_ptr(new Client{Object(OBJECT_TYPE::PLAYER, PLAYER_STATE::IDLE, {0, 0})});
    client_ptr->connect("127.0.0.1", 60'000);

    auto msg = net::Message<EVENT>{EVENT::NEW_PLAYER};
    client_ptr->send(msg);

    int step;
    while (true){
        std::cin >> step;
        client_ptr->move(step);
        client_ptr->update();
        std::cout.flush();
    }
}
