#pragma once

#include "std.h"

namespace wind {
class RenderConfig {
public:
    static constexpr u32 MRT_MAX_COUNT       = 10;
    static constexpr u32 MAX_FRAME_IN_FLIGHT = 2;
};
} // namespace wind