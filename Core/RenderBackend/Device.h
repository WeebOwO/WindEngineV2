#pragma once

#include "std.h"
#include "VulkanHeader.h"

#include "Allocator.h"
#include "Descriptor.h"

namespace wind {

struct QueueIndices {
    std::optional<uint32_t> graphicsQueueIndex;
    std::optional<uint32_t> computeQueueIndex;

    bool IsComplete() { return graphicsQueueIndex.has_value() && computeQueueIndex.has_value(); }
};

class VkAllocator;
class CommandEncoder;
class ShaderCache;

class GPUDevice {
public:
    friend class Backend;
    GPUDevice();
    ~GPUDevice();

    vk::Queue GetGraphicsQueue() const noexcept { return m_graphicsQueue; }
    vk::Queue GetComputeQueue() const noexcept { return m_computeQueue; }

    auto GetQueueIndices() noexcept { return m_queueIndices; }

    auto GetVkDeviceHandle() const noexcept { return *m_device; }
    auto GetVkPhysicalDevice() const noexcept { return m_physicalDevice; }
    auto GetVkInstance() const noexcept { return *m_vkInstance; }

    auto GetAllocator() const -> VkAllocator*;

    AllocatedBuffer AllocateBuffer(const vk::BufferCreateInfo&    bufferCreateInfo,
                                   const VmaAllocationCreateInfo& allocationCreateInfo) const;

    void DestroyBuffer(AllocatedBuffer& buffer) const;

    vk::DescriptorSet AllocateDescriptor(const vk::DescriptorSetLayout&) const;

    vk::CommandBuffer GetBackUpCommandBuffer();
    void SubmitBackUpCommandBuffer(const vk::CommandBuffer& buffer);

private:
    void InitAllocator();
    void CreateInstance();
    void PickupPhysicalDevice();
    void CreateDevice();
    void QueryQueueFamilyIndices();
    void InitBackupCommandBuffer();

    std::vector<const char*> GetRequiredExtensions();

    QueueIndices m_queueIndices;
    vk::Queue    m_graphicsQueue;
    vk::Queue    m_computeQueue;

    vk::UniqueInstance m_vkInstance;
    vk::PhysicalDevice m_physicalDevice;
    vk::UniqueDevice   m_device;

    vk::UniqueDebugUtilsMessengerEXT m_dubugMessenger;
    vk::DynamicLoader                m_vkLoader{};

    std::unordered_set<std::string> m_supportedExtensions;
    std::vector<const char*>        m_enableExtensions;

    std::unique_ptr<VkAllocator> m_allocator;
    bool                         m_enableDebug{true};

    std::unique_ptr<DescriptorAllocator> m_descriptorAllocator;

    vk::CommandPool   m_backupCommandPool;
    vk::CommandBuffer m_backupCommandBuffer;

    vk::Fence m_backupCommandfence;
};
} // namespace wind