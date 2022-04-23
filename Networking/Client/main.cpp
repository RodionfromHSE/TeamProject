#include "client.h"
#include "synchronizied.h"

int main() {
    std::shared_ptr<Client> client_ptr(new Client());
    client_ptr->connect("127.0.0.1", 60'000);

    net::Synchronized<Point, false> p(client_ptr, 770);
    int step;
    while (true){
        std::cin >> step;
        p.set(client_ptr->move(step));
        client_ptr->update();
        std::cout.flush();
    }
}
