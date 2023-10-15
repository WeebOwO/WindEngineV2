#pragma once

#include "std.h"

#include "Backend.h"

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

template <RHIResourceType ResourceType> 
struct ResourceHandle {
    static constexpr RHIResourceType handleType = ResourceType;
    u32                                 handle;
};

template <RHIResourceType ResourceType> 
struct RHIResource {
    RHIResource() : device(Backend::GetGPUDevice()) {}

    GPUDevice&                   device;
    ResourceHandle<ResourceType> handle;
};
} // namespace wind