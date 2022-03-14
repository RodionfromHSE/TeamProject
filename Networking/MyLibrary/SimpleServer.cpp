#include "connection.h"
#include "server.h"
#include "tsqueue.h"
#include "message.h"
#include "fwd.h"

enum class CustomMsgTypes : uint32_t
{
    ServerAccept,
    ServerDeny,
    ServerPing,
    MessageAll,
    ServerMessage,
};

using namespace myLibrary;

class CustomServer : public ServerInterface<CustomMsgTypes>
{
public:
    CustomServer(uint16_t nPort) : ServerInterface<CustomMsgTypes>(nPort)
    {

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
    void handle_message(std::shared_ptr<Connection<CustomMsgTypes>> client, Message <CustomMsgTypes>& msg) override
    {
        switch (msg.header.id)
        {
            case CustomMsgTypes::ServerPing:
            {
                std::cout << "[" << client->get_id() << "]: Server Ping\n";

                // Simply bounce Message  back to client
                client->send(msg);
            }
                break;

            case CustomMsgTypes::MessageAll:
            {
                std::cout << "[" << client->get_id() << "]: Message  All\n";

                // Construct a new Message  and send it to all clients
                Message<CustomMsgTypes> msg;
                msg.header.id = CustomMsgTypes::ServerMessage;
                msg << client->get_id();
                send_to_everyone(msg, client);

            }
                break;
        }
    }
};

int main()
{
    CustomServer server(60000);
    server.run();

    while (1)
    {
        server.update();
    }



    return 0;
}
