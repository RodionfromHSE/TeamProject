#pragma once

#include <vector>
#include <iostream>
#include "client_interface.h"
#include "object.h"
#include "game_enum.h"
#include "message.h"

struct Client {
    Client() = default;

    void move(int step);

    Object _player;
};