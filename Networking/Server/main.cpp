#include "server.h"
#include "synchronizied.h"
#include "players_keeper.h"

const int PORT = 1234;

int main() {
    std::shared_ptr<Server> server_ptr(new Server(PORT));
    PlayerKeeper playerKeeper(server_ptr, server_ptr->get_msgQueue());
    while (true) {
        server_ptr->update();
        playerKeeper.update();

        std::cout.flush();
    }
}
