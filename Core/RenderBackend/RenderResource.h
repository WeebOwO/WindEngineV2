#pragma once

#include "std.h"

#include "RenderBackend/Backend.h"

namespace wind {
class GPUDevice;

enum class RenderResourceType : u8 { Buffer, Texture, Shader, CommandPool };

template <RenderResourceType ResourceType> 
struct ResourceHandle {
    static constexpr RenderResourceType handleType = ResourceType;
    u32                                 handle;
};

template <RenderResourceType ResourceType> 
struct RenderResource {
    RenderResource() : device(Backend::GetGPUDevice()) {}
    GPUDevice&                   device;
    ResourceHandle<ResourceType> handle;
};
} // namespace wind