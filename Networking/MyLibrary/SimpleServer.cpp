#include "connection.h"
#include "server.h"
#include "tsqueue.h"
#include "message.h"
#include "fwd.h"

enum class SIGNAL{
    ServerPing
};

using namespace myLibrary;
struct Server : ServerInterface<SIGNAL>{
    using ServerInterface<SIGNAL>::ServerInterface;

    void handle_message(std::shared_ptr<Connection<SIGNAL>> client, Message<SIGNAL> &msg) override {
        if (msg.header.id == SIGNAL::ServerPing){
            std::cout << reinterpret_cast<char*>(&msg.body) << std::endl;
        }
    }
};

int main(){
    Server server(60000);
    server.run();
    while (true){
        server.update();
    }
    return 0;
}
