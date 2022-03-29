#include "client.h"


int main() {
    Client client{Object(OBJECT_TYPE::PLAYER, PLAYER_STATE::IDLE, {0, 0})};

    while (true){
        int step;
        std::cin >> step;
        client.move(step);

        while (!client.myLibrary::ClientInterface<OBJECT_TYPE>::inQueue.empty()); ????
    }
}
