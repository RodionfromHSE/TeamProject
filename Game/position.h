//
// Created by aleksandr on 10.03.2022.
//

#ifndef __PROJECT_POSITION_H
#define __PROJECT_POSITION_H

#include "game_object.h"

struct PositionComponent : Component {
    float x;
    float y;

    PositionComponent(int newX, int newY) : x(newX), y(newY) {}
};

#endif //__PROJECT_POSITION_H
