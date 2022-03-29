#include "client.h"

std::vector<Point> moves{ {1, 0}, {-1, 0}, {0, -1}, {0, 1} };

void Client::move(int step) {
    _player.set(moves[step]);
    myLibrary::Message<OBJECT_TYPE> msg;
    msg.header.id = OBJECT_TYPE::PLAYER;
    msg << _player.coordinates() << _player.state();
}

