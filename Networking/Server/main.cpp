#include "server.h"
#include "synchronizied.h"

int main() {
    std::shared_ptr<Server> server_ptr(new Server(60'000));
    net::Synchronized<int> i(server_ptr, 47);
    i.set(0);
    int prev = i.get();
    while (true) {
        server_ptr->update();
        if (i.get() != prev)
            prev = i.get(), std::cout << i.get() << std::endl;
        std::cout.flush();
    }
}
