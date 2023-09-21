#pragma once

#include <vk_mem_alloc.h>

#include "RenderBackend/Device.h"
#include "VulkanHeader.h"

namespace wind {
class VkAllocator {
public:
    auto allocator() const { return m_allocator; }
    VkAllocator(GPUDevice& device);
    ~VkAllocator();
    
private:
    VmaAllocator m_allocator;
};
} // namespace wind