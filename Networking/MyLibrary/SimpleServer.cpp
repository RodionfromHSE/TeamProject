#include "connection.h"
#include "server.h"
#include "tsqueue.h"
#include "message.h"
#include "fwd.h"

enum class CharacterAction : uint32_t {
    Move,
    SuccessfulMove
};

using namespace myLibrary;

class CustomServer : public ServerInterface<CharacterAction> {
public:
    CustomServer(uint16_t nPort) : ServerInterface<CharacterAction>(nPort) {

    }

protected:
//    virtual bool OnClientConnect(std::shared_ptr<Connection<CustomMsgTypes>> client)
//    {
//        Message <CustomMsgTypes> msg;
//        msg.header.id = CustomMsgTypes::ServerAccept;
//        client->send(msg);
//        return true;
//    }
//
//    // Called when a client appears to have disconnected
//    virtual void OnClientDisconnect(std::shared_ptr<Connection<CustomMsgTypes>> client)
//    {
//        std::cout << "Removing client [" << client->get_id() << "]\n";
//    }

    // Called when a Message  arrives
    void handle_message(std::shared_ptr<Connection<CharacterAction>> client, Message<CharacterAction> &msg) override {
        switch (msg.header.id) {
            case CharacterAction::Move: {
                std::pair<float, float> startPoint;
                std::pair<float, float> endPoint;
                msg >> startPoint >> endPoint;
                std::cout << "[" << client->get_id() << "]: Move from: " << startPoint.first << ' ' << startPoint.second
                          << " to " << endPoint.first << ' ' << endPoint.second << '\n';

                // Simply bounce Message  back to client
                msg.clear();
                msg.header.id = CharacterAction::SuccessfulMove;
                client->send(msg);
            }
                break;
        }
    }
};

int main() {
    CustomServer server(60000);
    server.run();

    while (1) {
        server.update();
        std::cout.flush();
    }


    return 0;
}
