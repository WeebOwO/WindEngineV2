#pragma once

#include <vulkan/vulkan.hpp>

namespace wind {
    
class GPUBuffer {
public:
    
private:
    vk::Buffer m_handle;
    vk::DeviceMemory m_memory;
};


} // namespace wind