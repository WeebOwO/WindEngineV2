#pragma once

#include "std.h"

#include "Backend.h"

namespace wind {
class GPUDevice;

enum class RenderResourceType : u8 {
    Buffer,
    Texture,
    Shader,
    CommandEncoder,
    CommandBuffer,
    ShaderParamer
};

template <RenderResourceType ResourceType> 
struct ResourceHandle {
    static constexpr RenderResourceType handleType = ResourceType;
    u32                                 handle;
};

template <RenderResourceType ResourceType> 
struct RHIResource {
    RHIResource() : device(Backend::GetGPUDevice()) {}

    GPUDevice&                   device;
    ResourceHandle<ResourceType> handle;
};
} // namespace wind