#pragma once

#include "std.h"

#include "Allocator.h"
#include "Descriptor.h"
#include "VulkanHeader.h"

namespace wind {

class VkAllocator;
class CommandEncoder;

struct QueueIndices {
    std::optional<uint32_t> graphicsQueueIndex;
    std::optional<uint32_t> computeQueueIndex;
    std::optional<uint32_t> asyncComputeQueueIndex;

    bool IsComplete() {
        return graphicsQueueIndex.has_value() && computeQueueIndex.has_value() &&
               asyncComputeQueueIndex.has_value();
    }
};

class GPUDevice {
public:
    GPUDevice();
    ~GPUDevice();

    operator vk::Device() { return *m_device; }

    vk::Queue GetGraphicsQueue() const noexcept { return m_graphicsQueue; }
    vk::Queue GetComputeQueue() const noexcept { return m_computeQueue; }
    vk::Queue GetAsyncComputeQueue() const noexcept { return m_asyncComputeQueue; };

    auto GetQueueIndices() const noexcept { return m_queueIndices; }

    auto GetVkDeviceHandle() const noexcept { return *m_device; }
    auto GetVkPhysicalDevice() const noexcept { return m_physicalDevice; }
    auto GetVkInstance() const noexcept { return *m_vkInstance; }

    auto GetAllocator() const -> VkAllocator*;
    auto GetLimits() { return m_limits; }

    AllocatedBuffer AllocateBuffer(const vk::BufferCreateInfo&    bufferCreateInfo,
                                   const VmaAllocationCreateInfo& allocationCreateInfo) const;

    void DestroyBuffer(AllocatedBuffer& buffer) const;

    AllocatedImage AllocateImage(const vk::ImageCreateInfo&     imageCreateInfo,
                                 const VmaAllocationCreateInfo& allocationCreateInfo) const;

    void DestroyImage(AllocatedImage& image) const;

    vk::DescriptorSet AllocateDescriptor(const vk::DescriptorSetLayout&) const;

    // block style submit
    vk::CommandBuffer GetBackUpCommandBuffer();
    void              SubmitBackUpCommandBuffer(const vk::CommandBuffer& buffer);

private:
    void InitAllocator();
    void CreateInstance();
    void PickupPhysicalDevice();
    void CreateDevice();
    void QueryQueueFamilyIndices();
    void InitBackupCommandBuffer();

    std::vector<const char*> GetRequiredExtensions();

    u32          m_alignSize;
    QueueIndices m_queueIndices;

    vk::Queue m_graphicsQueue;
    vk::Queue m_computeQueue;
    vk::Queue m_asyncComputeQueue;

    vk::UniqueInstance       m_vkInstance;
    vk::PhysicalDevice       m_physicalDevice;
    vk::PhysicalDeviceLimits m_limits;
    vk::UniqueDevice         m_device;

    vk::UniqueDebugUtilsMessengerEXT m_dubugMessenger;
    vk::DynamicLoader                m_vkLoader{};

    std::unordered_set<std::string> m_supportedExtensions;
    std::vector<const char*>        m_enableExtensions;

    Scope<VkAllocator> m_allocator;
    bool               m_enableDebug{true};

    vk::CommandPool   m_backupCommandPool;
    vk::CommandBuffer m_backupCommandBuffer;

    vk::Fence m_backupCommandfence;
};
} // namespace wind