#pragma once

#include <vk_mem_alloc.h>

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

class GPUDevice;
class VkAllocator {
public:
    auto& NativeHandle() const { return m_allocator; }
    VkAllocator(GPUDevice& device);
    ~VkAllocator();

    AllocatedBuffer AllocateBuffer(const vk::BufferCreateInfo& bufferCreateInfo, const VmaAllocationCreateInfo& allocationCreateInfo);
    void DestroyBuffer(AllocatedBuffer& buffer);
    
private:
    VmaAllocator m_allocator;
};
} // namespace wind