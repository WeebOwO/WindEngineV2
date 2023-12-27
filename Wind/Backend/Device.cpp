#include "Device.h"

#include <GLFW/glfw3.h>

#include "Shader.h"

#include "Core/Log.h"

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

namespace wind
{
    static std::vector<const char*> layers = {"VK_LAYER_KHRONOS_validation"};

    static std::vector<const char*> rayTracingExtensions = {VK_KHR_RAY_QUERY_EXTENSION_NAME,
                                                            VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME,
                                                            VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME,
                                                            VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME};

    VKAPI_ATTR VkBool32 VKAPI_CALL
    DebugUtilsMessengerCallback(VkDebugUtilsMessageSeverityFlagBitsEXT      messageSeverity,
                                VkDebugUtilsMessageTypeFlagsEXT             messageTypes,
                                VkDebugUtilsMessengerCallbackDataEXT const* pCallbackData,
                                void* /*pUserData*/)
    {
        WIND_CORE_INFO("{}", pCallbackData->pMessage);
        return false;
    }

    vk::DebugUtilsMessengerCreateInfoEXT MakeDebugUtilsMessengerCreateInfoEXT()
    {
        return {.sType           = vk::StructureType::eDebugUtilsMessengerCreateInfoEXT,
                .messageSeverity = vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
                                   vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
                .messageType = vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
                               vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
                               vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation,
                .pfnUserCallback = &DebugUtilsMessengerCallback};
    }

    std::vector<const char*> GPUDevice::GetRequiredExtensions()
    {
        uint32_t     glfwEextensionsCnt = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwEextensionsCnt);
        std::vector<const char*> extensions(glfwEextensionsCnt);

        for (int i = 0; i < glfwEextensionsCnt; ++i)
        {
            extensions[i] = glfwExtensions[i];
        }

        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        return extensions;
    }

    void GPUDevice::WaitIdle() { m_device->waitIdle(); }

    void GPUDevice::CreateInstance()
    {
        PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr =
            m_vkLoader.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");
        VULKAN_HPP_DEFAULT_DISPATCHER.init(vkGetInstanceProcAddr);

        vk::ApplicationInfo applicationInfo {.pApplicationName   = "App",
                                             .applicationVersion = 1,
                                             .pEngineName        = "Engine",
                                             .engineVersion      = 1,
                                             .apiVersion         = VK_API_VERSION_1_3};

        auto extensions = GetRequiredExtensions();

        vk::InstanceCreateInfo instanceCreateInfo {
            .flags                   = {},
            .pApplicationInfo        = &applicationInfo,
            .enabledLayerCount       = (uint32_t)layers.size(),
            .ppEnabledLayerNames     = layers.data(),
            .enabledExtensionCount   = (uint32_t)extensions.size(),
            .ppEnabledExtensionNames = extensions.data(),
        };

        m_vkInstance = vk::createInstanceUnique(instanceCreateInfo, nullptr);
        VULKAN_HPP_DEFAULT_DISPATCHER.init(*m_vkInstance);

        m_dubugMessenger =
            m_vkInstance->createDebugUtilsMessengerEXTUnique(MakeDebugUtilsMessengerCreateInfoEXT(), nullptr);
    }

    void GPUDevice::PickupPhysicalDevice()
    {
        m_physicalDevice = m_vkInstance->enumeratePhysicalDevices().front();

        auto properties = m_physicalDevice.getProperties();
        m_limits        = properties.limits;

        WIND_CORE_INFO(properties.deviceName);

        auto supportedExtensions = m_physicalDevice.enumerateDeviceExtensionProperties();

        for (const auto extension : supportedExtensions)
        {
            m_supportedExtensions.insert(extension.extensionName);
        }

        m_enableExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

        m_enableExtensions.insert(m_enableExtensions.end(), rayTracingExtensions.begin(), rayTracingExtensions.end());

        m_enableExtensions.erase(std::remove_if(m_enableExtensions.begin(),
                                                m_enableExtensions.end(),
                                                [&](const char* ptr) {
                                                    if (!m_supportedExtensions.contains(ptr))
                                                    {
                                                        WIND_CORE_ERROR("{} is not support", ptr);
                                                        return true;
                                                    }
                                                    return false;
                                                }),
                                 m_enableExtensions.end());

        std::for_each(m_enableExtensions.begin(), m_enableExtensions.end(), [](const char* ptr) {
            WIND_CORE_INFO("Open extension {}", ptr);
        });
    }

    void GPUDevice::QueryQueueFamilyIndices()
    {

        auto queueProperties = m_physicalDevice.getQueueFamilyProperties();

        for (uint32_t i = 0; const auto& queueFamily : queueProperties)
        {
            if (queueFamily.queueCount > 0 && queueFamily.queueFlags & vk::QueueFlagBits::eGraphics)
            {
                m_queueIndices.graphicsQueueIndex = i;
            }

            if (queueFamily.queueCount > 0 && queueFamily.queueFlags & vk::QueueFlagBits::eCompute)
            {
                if (m_queueIndices.graphicsQueueIndex.has_value() && i != m_queueIndices.graphicsQueueIndex)
                {
                    m_queueIndices.asyncComputeQueueIndex = i;
                }
                else
                {
                    m_queueIndices.computeQueueIndex = i;
                }
            }

            if (m_queueIndices.IsComplete())
                break;
            ++i;
        }
    }

    void GPUDevice::CreateDevice()
    {
        // Create Device
        std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;

        std::unordered_set<uint32_t> uniqueQueueIndices {m_queueIndices.graphicsQueueIndex.value(),
                                                         m_queueIndices.computeQueueIndex.value(),
                                                         m_queueIndices.asyncComputeQueueIndex.value()};

        vk::PhysicalDeviceDynamicRenderingFeaturesKHR dynamic_rendering = {};
        dynamic_rendering.dynamicRendering                              = true;

        float queuePriority = 1.0f;

        for (auto index : uniqueQueueIndices)
        {
            vk::DeviceQueueCreateInfo queueCreateInfo {
                .queueFamilyIndex = index, .queueCount = 1, .pQueuePriorities = &queuePriority};
            queueCreateInfos.push_back(queueCreateInfo);
        }

        vk::DeviceCreateInfo deviceCreateInfo {
            .queueCreateInfoCount    = (uint32_t)queueCreateInfos.size(),
            .pQueueCreateInfos       = queueCreateInfos.data(),
            .enabledExtensionCount   = (uint32_t)m_enableExtensions.size(),
            .ppEnabledExtensionNames = m_enableExtensions.data(),
        };

        deviceCreateInfo.setPNext(&dynamic_rendering);

        m_device = m_physicalDevice.createDeviceUnique(deviceCreateInfo);
        VULKAN_HPP_DEFAULT_DISPATCHER.init(*m_device);
        // Get queue
        m_graphicsQueue     = m_device->getQueue(m_queueIndices.graphicsQueueIndex.value(), 0);
        m_computeQueue      = m_device->getQueue(m_queueIndices.computeQueueIndex.value(), 0);
        m_asyncComputeQueue = m_device->getQueue(m_queueIndices.asyncComputeQueueIndex.value(), 0);
    }

    void GPUDevice::InitAllocator() { m_allocator = scope::Create<VkAllocator>(*this); }

    VkAllocator* GPUDevice::GetAllocator() const { return m_allocator.get(); }

    GPUDevice::GPUDevice()
    {
        CreateInstance();
        PickupPhysicalDevice();
        QueryQueueFamilyIndices();
        CreateDevice();
        InitAllocator();
        InitBackupCommandBuffer();
    }

    GPUDevice::~GPUDevice()
    {
        m_device->waitIdle();
        m_device->destroyCommandPool(m_backupCommandPool);
        m_device->destroyFence(m_backupCommandfence);
    }

    // buffer interface
    AllocatedBuffer GPUDevice::AllocateBuffer(const vk::BufferCreateInfo&    bufferCreateInfo,
                                              const VmaAllocationCreateInfo& allocationCreateInfo) const
    {
        return m_allocator->AllocateBuffer(bufferCreateInfo, allocationCreateInfo);
    }

    void GPUDevice::DestroyBuffer(AllocatedBuffer& buffer) const { m_allocator->DestroyBuffer(buffer); }

    AllocatedImage GPUDevice::AllocateImage(const vk::ImageCreateInfo&     imageCreateInfo,
                                            const VmaAllocationCreateInfo& allocationCreateInfo) const
    {
        return m_allocator->AllocateImage(imageCreateInfo, allocationCreateInfo);
    }

    void GPUDevice::DestroyImage(AllocatedImage& image) const { m_allocator->DestroyImage(image); }

    void GPUDevice::InitBackupCommandBuffer()
    {
        vk::CommandPoolCreateInfo poolCreateInfo {.queueFamilyIndex = m_queueIndices.graphicsQueueIndex.value()};

        m_backupCommandPool = m_device->createCommandPool(poolCreateInfo);

        vk::CommandBufferAllocateInfo allocateInfo {
            .commandPool = m_backupCommandPool, .level = vk::CommandBufferLevel::ePrimary, .commandBufferCount = 1};

        m_backupCommandBuffer = m_device->allocateCommandBuffers(allocateInfo).front();

        vk::FenceCreateInfo fenceCreateInfo {};
        m_backupCommandfence = m_device->createFence(fenceCreateInfo);
    }

    vk::CommandBuffer GPUDevice::GetBackUpCommandBuffer() { return m_backupCommandBuffer; }

    void GPUDevice::SubmitBackUpCommandBuffer(const vk::CommandBuffer& buffer)
    {
        buffer.end();

        vk::SubmitInfo submitInfo {.commandBufferCount = 1, .pCommandBuffers = &buffer};
        m_graphicsQueue.submit(submitInfo, m_backupCommandfence);

        auto result = m_device->waitForFences(m_backupCommandfence, true, std::numeric_limits<float>::max());
        if (result != vk::Result::eSuccess)
        {
            WIND_CORE_WARN("Backup ComandBuffer wait too long time");
        }
        m_device->resetFences(m_backupCommandfence);

        m_device->resetCommandPool(m_backupCommandPool);
    }
}; // namespace wind