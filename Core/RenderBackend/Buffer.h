#pragma once

#include "VulkanHeader.h"

namespace wind {

struct BufferDesc {};

class GPUBuffer {
public:
private:
    vk::Buffer       m_handle;
    vk::DeviceMemory m_memory;
};

} // namespace wind