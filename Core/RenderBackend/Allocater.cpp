#define VMA_IMPLEMENTATION

#include "Allocator.h"
#include "Base/Log.h"

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
} // namespace wind
