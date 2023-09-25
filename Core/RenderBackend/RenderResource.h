#pragma once

#include "std.h"

#include "RenderBackend/Backend.h"

namespace wind {
class GPUDevice;

enum class RenderResourceType : u8 { Buffer, Texture, Shader };

template <RenderResourceType ResourceType> 
struct ResourceHandle {
    u64 handle;
};

template <RenderResourceType ResourceType> 
struct RenderResource {
    RenderResource(): device(Backend::GetGPUDevice()) {}
    GPUDevice&                   device;
    ResourceHandle<ResourceType> handle;
};

} // namespace wind