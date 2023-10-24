#pragma once

namespace wind {
struct RenderResourceInterface {
    virtual void InitRHI() const = 0;
    virtual void RelaseRHI() const = 0;
};
} // namespace wind