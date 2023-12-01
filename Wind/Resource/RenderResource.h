#pragma once

namespace wind {
struct RenderResource {
    virtual void InitRHI() {}
    virtual void ReleaseRHI() {}
};
} // namespace wind