#pragma once

#include "VulkanHeader.h"

namespace wind {
    
class GPUBuffer {
public:
    
private:
    vk::Buffer m_handle;
    vk::DeviceMemory m_memory;
};

} // namespace wind