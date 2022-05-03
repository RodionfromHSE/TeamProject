#pragma once

#include "game_enum.h"

struct Point{
    int x, y;
};

//Point operator+(const Point& first, const Point& second);
//
//struct Object{
//    explicit Object(OBJECT_TYPE type, PLAYER_STATE state, Point coors);
//
//    void set(Point coors, PLAYER_STATE state=PLAYER_STATE::IDLE);
//
//    [[nodiscard]] const Object& get() const;
//
//    [[nodiscard]] const Point& coordinates() const;
//
//    [[nodiscard]] const PLAYER_STATE& state() const;
//
//private:
//    OBJECT_TYPE _type;
//    PLAYER_STATE _state;
//    Point _coors;
//};
//
//
