#pragma once

#include "VulkanHeader.h"

#include "RenderBackend/Device.h"
#include "RenderBackend/RenderResource.h"

namespace wind {

struct BufferDesc {};

class GPUBuffer : public RenderResource {
public:
    GPUBuffer();
private:
    vk::Buffer       m_handle;
    size_t           m_byteSize;
};

} // namespace wind