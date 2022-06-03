#include "server.h"
#include "synchronizied.h"
#include "players_keeper.h"

int main() {
    std::shared_ptr<Server> server_ptr(new Server(60'000));
    PlayerKeeper playerKeeper(server_ptr, server_ptr->get_msgQueue());
    while (true) {
        server_ptr->update();
        playerKeeper.update();

        std::cout.flush();
    }
}
