#pragma once

#include <memory>
#include <optional>
#include <thread>
#include <unordered_set>

#include "RenderBackend/Image.h"
#include "VulkanHeader.h"

#include "RenderBackend/Command.h"
#include "RenderBackend/Encoder.h"
#include "RenderBackend/Pipeline.h"
#include "RenderBackend/Buffer.h"

namespace wind {

struct QueueIndices {
    std::optional<uint32_t> graphicsQueueIndex;
    std::optional<uint32_t> computeQueueIndex;

    bool IsComplete() { return graphicsQueueIndex.has_value() && computeQueueIndex.has_value(); }
};

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

    // Crate interface
    std::unique_ptr<CommandEncoder>
    CreateCommandEncoder(RenderCommandQueueType queueType = RenderCommandQueueType::Graphics);
    
    std::unique_ptr<GPUTexture> CreateTexture(const TextureDesc& textureDesc);
    std::unique_ptr<GPUBuffer> CreateBuffer();
    // Destory Interface
    void DestroyCommandEncoder(CommandEncoder& encoder);

private:
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

    bool m_enableDebug{true};
};
} // namespace wind