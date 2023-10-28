#pragma once

namespace wind {
struct RenderResourceInterface {
    virtual void InitRHI()   = 0;
    virtual void RelaseRHI() = 0;
};
} // namespace wind