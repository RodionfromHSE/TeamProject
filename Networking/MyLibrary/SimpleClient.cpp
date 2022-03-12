#include "connection.h"
#include "client.h"
#include "tsqueue.h"
#include "message.h"
#include "fwd.h"

enum class SIGNAL {
    ServerPing
};

using namespace myLibrary;

struct Client : ClientInterface<SIGNAL> {
    void ping() {
        if (is_connected()) {
            Message<SIGNAL> msg;
            msg.header.id = SIGNAL::ServerPing;
            msg << "Message " << ++_msgCount;

            send(msg);
        }
    }

private:
    uint32_t _msgCount = 0;
};

int main() {
    Client c;
    c.connect("127.0.0.1", 60000);
    for (int i = 0; i < 100; ++i) {
        c.ping();
    }
    while (true) ;

    return 0;
}
