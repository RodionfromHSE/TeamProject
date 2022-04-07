#include "server.h"
#include "synchronizied.h"

int main() {
    std::shared_ptr<Server> server_ptr(new Server(60'000));
    while (true) {
        server_ptr->update();
        std::cout.flush();
    }
}
