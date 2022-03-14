#include "connection.h"
#include "client.h"
#include "tsqueue.h"
#include "message.h"
#include "fwd.h"
#include <chrono>
#include <thread>

enum class CustomMsgTypes : uint32_t
{
    ServerAccept,
    ServerDeny,
    ServerPing,
    MessageAll,
    ServerMessage,
};

using namespace myLibrary;

class CustomClient : public ClientInterface<CustomMsgTypes>
{
public:
    void PingServer()
    {
        Message<CustomMsgTypes> msg;
        msg.header.id = CustomMsgTypes::ServerPing;

        // Caution with this...
        std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();

        msg << timeNow;
        send(msg);
    }

    void MessageAll()
    {
        Message<CustomMsgTypes> msg;
        msg.header.id = CustomMsgTypes::MessageAll;
        send(msg);
    }
};

int main()
{
    CustomClient c;
    c.connect("127.0.0.1", 60000);

    bool bQuit = false;
    while (!bQuit)
    {
        char symb;
        std::cin >> symb;

        if (symb == '1') c.PingServer();
        else if (symb == '2') c.MessageAll();
        else if (symb == '3') bQuit = true;

        if (c.is_connected())
        {
            if (!c.inQueue.empty())
            {


                auto msg = c.inQueue.pop_front().msg;

                switch (msg.header.id)
                {
                    case CustomMsgTypes::ServerAccept:
                    {
                        // Server has responded to a ping request				
                        std::cout << "Server Accepted Connection\n";
                    }
                        break;


                    case CustomMsgTypes::ServerPing:
                    {
                        // Server has responded to a ping request
                        std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();
                        std::chrono::system_clock::time_point timeThen;
                        msg >> timeThen;
                        std::cout << "Ping: " << std::chrono::duration<double>(timeNow - timeThen).count() << "\n";
                    }
                        break;

                    case CustomMsgTypes::ServerMessage:
                    {
                        // Server has responded to a ping request	
                        uint32_t clientID;
                        msg >> clientID;
                        std::cout << "Hello from [" << clientID << "]\n";
                    }
                        break;
                }
            }
        }
        else
        {
            std::cout << "Server Down\n";
            bQuit = true;
        }

    }

    return 0;
}
