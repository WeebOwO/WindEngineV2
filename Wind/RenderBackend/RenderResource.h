#pragma once

#include "std.h"

#include "Engine/RuntimeContext.h"

namespace wind {
class GPUDevice;

enum class RHIResourceType : u8 {
    Buffer,
    Texture,
    Shader,
    CommandEncoder,
    CommandBuffer,
    ShaderParamer,
    RenderPass
};

template <RHIResourceType ResourceType> struct ResourceHandle {
    static constexpr RHIResourceType handleType = ResourceType;
    u32                              handle;
};

template <RHIResourceType ResourceType> struct RHIResource {
    RHIResource() : device(*g_runtimeContext.device) {}

    GPUDevice&                   device;
    ResourceHandle<ResourceType> handle;
};
} // namespace wind