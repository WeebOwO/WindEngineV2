#pragma once

#include "std.h"

#include "Engine/RuntimeContext.h"

namespace wind {
class GPUDevice;

enum class RHIResourceType : uint8_t {
    Buffer,
    Texture,
    Shader,
    CommandEncoder,
    CommandBuffer,
    ShaderParamer,
    RenderPass,
    Pipeline
};

template <RHIResourceType ResourceType> 
struct ResourceHandle {
    static constexpr RHIResourceType handleType = ResourceType;
    uint32_t                              handle;
};

template <RHIResourceType ResourceType> 
struct RHIResource {
    RHIResource() : device(*g_runtimeContext.device) {}

    GPUDevice&                   device;
    ResourceHandle<ResourceType> handle;
};
} // namespace wind