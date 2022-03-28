#include "client.h"

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
        msg << p.get_position() << p.move(1, 1);
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

}
