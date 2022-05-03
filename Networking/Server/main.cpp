#include "server.h"
#include "synchronizied.h"

int main() {
    std::shared_ptr<Server> server_ptr(new Server(60'000));
    net::Synchronized<Point> p(server_ptr, 770);

    net::Message<EVENT> msg;
    msg.header.id = EVENT::NOTHING;
    while (true) {
        server_ptr->update(); // TODO
        if (p.isUpdatable())
            std::cout << p.get().x << ' ' << p.get().y << '\n', server_ptr->send_to_everyone(msg);
        else
            std::cout << /*p.get().x <<*/ '-' /*<< p.get().y */<< '\n';
        std::cout.flush();
    }
}
