#pragma once

#include <vk_mem_alloc.h>

#include "RenderBackend/Device.h"
#include "VulkanHeader.h"

namespace wind {
struct AllocatedBuffer {
    vk::Buffer    buffer;
    VmaAllocation allocation;
};

struct AllocatedImage {
    vk::Image     image;
    VmaAllocation allocation;
};

class VkAllocator {
public:
    auto allocator() const { return m_allocator; }
    VkAllocator(GPUDevice& device);
    ~VkAllocator();

    AllocatedBuffer AllocateBuffer(const vk::BufferCreateInfo& bufferCreateInfo, const VmaAllocationCreateInfo& allocationCreateInfo);
    
private:
    VmaAllocator m_allocator;
};
} // namespace wind