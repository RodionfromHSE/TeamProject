//
// Created by aleksandr on 10.03.2022.
//

#ifndef __PROJECT_POSITION_H
#define __PROJECT_POSITION_H

#include "game_object.h"

struct PositionComponent : Component {
    int x;
    int y;

    PositionComponent(int newX, int newY) : x(newX), y(newY) {}

    ~PositionComponent() override;
};

PositionComponent::~PositionComponent() = default;

#endif //__PROJECT_POSITION_H
