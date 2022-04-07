#pragma once

#include "game_object.h"

struct PositionComponent : Component {
    int x;
    int y;

    PositionComponent(int newX, int newY) : x(newX), y(newY) {}

    ~PositionComponent() override;
};

PositionComponent::~PositionComponent() = default;
