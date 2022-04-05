#pragma once

#include <cstdint>

enum class OBJECT_TYPE : uint32_t
{
    PLAYER,
    DECORATION
};

enum class EVENT : uint32_t {
    SYNCHRONIZATION,
    NOTHING
};

enum class PLAYER_STATE : uint32_t
{
    RUN,
    IDLE,
    JUMP
};


