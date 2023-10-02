#pragma once

#include "std.h"
#include "VulkanHeader.h"
#include "RenderBackend/Allocator.h"
#include "RenderBackend/Descriptor.h"

namespace wind {

struct QueueIndices {
    std::optional<uint32_t> graphicsQueueIndex;
    std::optional<uint32_t> computeQueueIndex;

    bool IsComplete() { return graphicsQueueIndex.has_value() && computeQueueIndex.has_value(); }
};

class VkAllocator;
class GPUDevice {
public:
    friend class Backend;
    GPUDevice();
    ~GPUDevice();

    vk::Queue GetGraphicsQueue() { return m_graphicsQueue; }
    vk::Queue GetComputeQueue() { return m_computeQueue; }

    auto GetQueueIndices() { return m_queueIndices; }

    auto GetVkDeviceHandle() const { return *m_device; }
    auto GetVkPhysicalDevice() const { return m_physicalDevice; }
    auto GetVkInstance() const { return *m_vkInstance; }

    auto GetAllocator() const -> VkAllocator*;

    AllocatedBuffer AllocateBuffer(const vk::BufferCreateInfo&    bufferCreateInfo,
                                   const VmaAllocationCreateInfo& allocationCreateInfo);
    
    void            DeAllocateBuffer(AllocatedBuffer& buffer);

    vk::DescriptorSet AllocateDescriptor(const vk::DescriptorSetLayout& );

private:
    void InitAllocator();
    void CreateInstance();
    void PickupPhysicalDevice();
    void CreateDevice();
    void QueryQueueFamilyIndices();

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
};
} // namespace wind