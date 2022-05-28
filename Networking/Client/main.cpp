#include "client.h"
#include "synchronizied.h"
#include "client_interface.h"


int main() {
    std::shared_ptr<Client> client_ptr(new Client());
    client_ptr->connect("127.0.0.1", 60'000);

    net::Synchronized<Point, Usage::OnClient> p(client_ptr, 770);
    int step;
    auto msg = net::Message<EVENT>();
    msg.header.id = EVENT::NOTHING;

    std::thread thread([&]() {
        while (true) {
            client_ptr->update();
            std::cout.flush();
        }
    });


    while (true) {
        if (client_ptr->is_connected()) {
            if (p.isUpdatable())
                std::cout << "Coordinates from remote: " << p.get().x << ' ' << p.get().y << '\n';
            else
                std::cout << "Current coordinates: " << p.get().x << ' ' << p.get().y << '\n';
            std::cin >> step;
            //        client_ptr->send(msg);
            p.set({step, step});
            std::cout.flush();
        } else
            break;
    }
    thread.detach();
}
