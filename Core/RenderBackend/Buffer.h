#pragma once

#include "VulkanHeader.h"

#include "RenderBackend/Device.h"
#include "RenderBackend/RenderResource.h"

namespace wind {

struct BufferDesc {};

class GPUBuffer : public RenderResource<RenderResourceType::Buffer> {
public:
    GPUBuffer();

private:
    vk::Buffer m_buffer;
    u32        m_byteSize;
};

} // namespace wind