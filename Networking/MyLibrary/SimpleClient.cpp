#include "connection.h"
#include "client.h"
#include "tsqueue.h"
#include "message.h"
#include "fwd.h"
#include <chrono>
#include <thread>

enum class CharacterAction : uint32_t
{
    Move,
    SuccessfulMove
};

using namespace myLibrary;

struct Player{
    int getOx(){
        return i++;
    }

    std::pair<float, float> get_position(){
        return std::make_pair(x, y);
    }

    std::pair<float, float> move(float dx, float dy){
        x += dx, y += dy;
        return std::make_pair(x, y);
    }
private:
    int i = 0;
    float x = 0, y = 0;

};


class CustomClient : public ClientInterface<CharacterAction>
{
public:
    void PingServer(Player& p)
    {
        Message<CharacterAction> msg;
        msg.header.id = CharacterAction::Move;
        p.move(1, 0);
        msg << p.getOx();
        send(msg);
    }

//    void MessageAll()
//    {
//        Message<CharacterAction> msg;
//        msg.header.id = CharacterAction::MessageAll;
//        send(msg);
//    }
};

int main()
{
    CustomClient c;
    c.connect("127.0.0.1", 60000);
    Player p;
    bool bQuit = false;
    while (!bQuit)
    {
        char symb;
        std::cin >> symb;

        if (symb == '1') c.PingServer(p);
//        else if (symb == '2') c.MessageAll();
        else if (symb == '3') bQuit = true;

        if (c.is_connected())
        {
            c.inQueue.wait();
            while (!c.inQueue.empty())
            {
                auto msg = c.inQueue.pop_front().msg;

                switch (msg.header.id)
                {
                    case CharacterAction::SuccessfulMove:
                    {
                        std::cout << "Yoho\n";
                        std::cout.write(reinterpret_cast<char*>(&msg.body), msg.body.size());
                    }
                        break;
                }
            }
            std::cout.flush();
        }
        else
        {
            std::cout << "Server Down\n";
            bQuit = true;
        }
    }

    return 0;
}
