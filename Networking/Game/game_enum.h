#pragma once

#include <cstdint>

namespace con_expr {
    const int idPlayer1 = 46;
    const int idPlayer2 = 47;

    const int port = 60'000;
}
enum class EVENT : uint32_t {
    SYNCHRONIZATION,
    NOTHING,
    NEW_PLAYER
};



