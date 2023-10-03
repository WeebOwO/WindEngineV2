#define VMA_IMPLEMENTATION

#include "Allocator.h"

#include "Base/Log.h"
#include "RenderBackend/Device.h"

namespace wind {
VkAllocator::VkAllocator(GPUDevice& device) {
    VmaAllocatorCreateInfo createInfo{.flags                       = {},
                                      .physicalDevice              = device.GetVkPhysicalDevice(),
                                      .device                      = device.GetVkDeviceHandle(),
                                      .preferredLargeHeapBlockSize = 0,
                                      .pAllocationCallbacks        = nullptr,
                                      .pDeviceMemoryCallbacks      = nullptr,
                                      .pHeapSizeLimit              = nullptr,
                                      .pVulkanFunctions            = nullptr,
                                      .instance                    = device.GetVkInstance(),
                                      .vulkanApiVersion            = VK_API_VERSION_1_3,
                                      .pTypeExternalMemoryHandleTypes = nullptr};

    vmaCreateAllocator(&createInfo, &m_allocator);
    WIND_CORE_INFO("Create vulkan memory allocator");
}

VkAllocator::~VkAllocator() { vmaDestroyAllocator(m_allocator); }

AllocatedBuffer VkAllocator::AllocateBuffer(const vk::BufferCreateInfo&    bufferCreateInfo,
                                            const VmaAllocationCreateInfo& allocationCreateInfo) {
    AllocatedBuffer buffer;
    vmaCreateBuffer(m_allocator, (VkBufferCreateInfo*)&bufferCreateInfo, &allocationCreateInfo,
                    (VkBuffer*)&buffer.buffer, &buffer.allocation, nullptr);
    return buffer;
}

void VkAllocator::DestroyBuffer(AllocatedBuffer& buffer) {
    vmaDestroyBuffer(m_allocator, buffer.buffer, buffer.allocation);
}
} // namespace wind
