#pragma once

#include "std.h"

namespace wind
{
    class RenderConfig
    {
    public:
        static constexpr uint32_t MRT_MAX_COUNT       = 10;
        static constexpr uint32_t MAX_FRAME_IN_FLIGHT = 2;
    };
} // namespace wind