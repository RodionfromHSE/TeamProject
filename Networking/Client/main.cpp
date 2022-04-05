#include "client.h"
#include "synchronizied.h"

int main() {
    std::shared_ptr<Client> client_ptr(new Client{Object(OBJECT_TYPE::PLAYER, PLAYER_STATE::IDLE, {0, 0})});
    client_ptr->connect("127.0.0.1", 60'000);
    int step;
    Point p;
    PLAYER_STATE ps;
    net::Synchronized<int, false> i(client_ptr, 47);
    while (true){
        std::cin >> step;
        i.set(step);
        std::cout.flush();
    }
}
