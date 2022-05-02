#include "client.h"
#include "synchronizied.h"

int main() {
    std::shared_ptr<Client> client_ptr(new Client());
    client_ptr->connect("127.0.0.1", 60'000);

    net::Synchronized<Point, false> p(client_ptr, 770);
    int step;
    auto msg = net::Message<EVENT>();
    msg.header.id = EVENT::NOTHING;

    while (true){
        std::cin >> step;
//        client_ptr->send(msg);
        p.set({step, step});
//        client_ptr->update(); // TODO
        std::cout.flush();
    }
}
