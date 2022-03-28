#pragma once

#include "game_enum.h"

struct Point{
    int x, y;
};

struct Object{
    explicit Object(OBJECT_TYPE type, PLAYER_STATE state, Point coors);

    void set(PLAYER_STATE state, Point coors);

    void get() const;

private:
    OBJECT_TYPE _type;
    PLAYER_STATE _state;
    Point _coors;
};
