#include "client.h"


int main() {
    Client client{Object(OBJECT_TYPE::PLAYER, PLAYER_STATE::IDLE, {0, 0})};
    client.connect("127.0.0.1", 60'000);

    int step;
    Point p;
    PLAYER_STATE ps;
    while (true){
        std::cin >> step;
        client.move(step);

        while (!client.inQueue.empty()) {
            auto msg = client.inQueue.pop_front().msg;
            std::cout << msg << ' ' << client.inQueue.size();
            msg >> p >> ps;
            std::cout << p.x << ' ' << p.y << '\n';
            switch (ps) {
                case PLAYER_STATE::IDLE:
                    std::cout << "IDLE\n";
                    break;
                case PLAYER_STATE::RUN:
                    std::cout << "RUN\n";
                    break;
                case PLAYER_STATE::JUMP:
                    std::cout << "JUMP\n";
                    break;
            }
        }
        if (!client.is_connected()){
            std::cout << "Goodbye America!\n";
            break;
        }
        std::cout.flush();
    }
}
