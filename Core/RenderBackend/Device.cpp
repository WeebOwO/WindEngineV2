#include "Device.h"

#include "Base/Log.h"
#include "RenderBackend/Encoder.h"

#include <GLFW/glfw3.h>
#include <algorithm>

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

namespace wind {
static std::vector<const char*> layers = {"VK_LAYER_KHRONOS_validation"};

static std::vector<const char*> rayTracingExtensions = {
    VK_KHR_RAY_QUERY_EXTENSION_NAME, VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME,
    VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME, VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME};

VKAPI_ATTR VkBool32 VKAPI_CALL DebugUtilsMessengerCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT      messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT             messageTypes,
    VkDebugUtilsMessengerCallbackDataEXT const* pCallbackData, void* /*pUserData*/) {
    WIND_CORE_INFO("{}", pCallbackData->pMessage);
    return false;
}

vk::DebugUtilsMessengerCreateInfoEXT MakeDebugUtilsMessengerCreateInfoEXT() {
    return {{},
            vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
                vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
            vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
                vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
                vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation,
            &DebugUtilsMessengerCallback};
}

std::vector<const char*> GPUDevice::GetRequiredExtensions() {
    uint32_t     glfwEextensionsCnt = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwEextensionsCnt);
    std::vector<const char*> extensions(glfwEextensionsCnt);

    for (int i = 0; i < glfwEextensionsCnt; ++i) {
        extensions[i] = glfwExtensions[i];
    }

    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    return extensions;
}

void GPUDevice::CreateInstance() {
    PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr =
        m_vkLoader.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");
    VULKAN_HPP_DEFAULT_DISPATCHER.init(vkGetInstanceProcAddr);

    vk::ApplicationInfo applicationInfo("App", 1, "Engine", 1, VK_API_VERSION_1_3);

    auto                   extensions = GetRequiredExtensions();
    vk::InstanceCreateInfo instanceCreateInfo({}, &applicationInfo, uint32_t(layers.size()),
                                              layers.data(), (uint32_t)extensions.size(),
                                              extensions.data());

    m_vkInstance = vk::createInstance(instanceCreateInfo, nullptr);
    VULKAN_HPP_DEFAULT_DISPATCHER.init(m_vkInstance);

    m_dubugMessenger =
        m_vkInstance.createDebugUtilsMessengerEXT(MakeDebugUtilsMessengerCreateInfoEXT(), nullptr);
}

void GPUDevice::PickupPhysicalDevice() {
    m_physicalDevice = m_vkInstance.enumeratePhysicalDevices().front();
    WIND_CORE_INFO(m_physicalDevice.getProperties().deviceName);

    auto supportedExtensions = m_physicalDevice.enumerateDeviceExtensionProperties();

    for (const auto extension : supportedExtensions) {
        m_supportedExtensions.insert(extension.extensionName);
    }

    m_enableExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
    m_enableExtensions.insert(m_enableExtensions.end(), rayTracingExtensions.begin(),
                              rayTracingExtensions.end());

    m_enableExtensions.erase(std::remove_if(m_enableExtensions.begin(), m_enableExtensions.end(),
                                            [&](const char* ptr) {
                                                if (!m_supportedExtensions.contains(ptr)) {
                                                    WIND_CORE_ERROR("{} is not support", ptr);
                                                    return true;
                                                }
                                                return false;
                                            }),
                             m_enableExtensions.end());

    std::for_each(m_enableExtensions.begin(), m_enableExtensions.end(),
                  [](const char* ptr) { WIND_CORE_INFO("Open extension {}", ptr); });
}

void GPUDevice::QueryQueueFamilyIndices() {

    auto queueProperties = m_physicalDevice.getQueueFamilyProperties();

    for (uint32_t i = 0; const auto& queueFamily : queueProperties) {
        if (queueFamily.queueCount > 0 && queueFamily.queueFlags & vk::QueueFlagBits::eGraphics) {
            m_queueIndices.graphicsQueueIndex = i;
        }

        if (queueFamily.queueCount > 0 && queueFamily.queueFlags & vk::QueueFlagBits::eCompute) {
            m_queueIndices.computeQueueIndex = i;
        }

        if (m_queueIndices.IsComplete()) break;
        ++i;
    }
}

void GPUDevice::CreateDevice() {
    // Create Device
    vk::DeviceCreateInfo                   deviceCreateInfo;
    std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;

    std::unordered_set<uint32_t> uniqueQueueIndices{m_queueIndices.graphicsQueueIndex.value(),
                                                    m_queueIndices.computeQueueIndex.value()};

    float queuePriority = 1.0f;

    for (auto index : uniqueQueueIndices) {
        vk::DeviceQueueCreateInfo queueCreateInfo{vk::DeviceQueueCreateFlags{}, index, 1,
                                                  &queuePriority};
        queueCreateInfos.push_back(queueCreateInfo);
    }

    deviceCreateInfo.setQueueCreateInfos(queueCreateInfos)
        .setPEnabledExtensionNames(m_enableExtensions);

    m_device = m_physicalDevice.createDevice(deviceCreateInfo);
    VULKAN_HPP_DEFAULT_DISPATCHER.init(m_device);
    // Get queue
    m_graphicsQueue = m_device.getQueue(m_queueIndices.graphicsQueueIndex.value(), 0);
    m_computeQueue  = m_device.getQueue(m_queueIndices.computeQueueIndex.value(), 0);
}

GPUDevice::GPUDevice() {
    CreateInstance();
    PickupPhysicalDevice();
    QueryQueueFamilyIndices();
    CreateDevice();
}

GPUDevice::~GPUDevice() {
    m_device.waitIdle();
    m_device.destroy();
    m_vkInstance.destroyDebugUtilsMessengerEXT(m_dubugMessenger);
    m_vkInstance.destroy();
}

void GPUDevice::DestroyCommandEncoder(CommandEncoder& encoder) {
   
}

}; // namespace wind