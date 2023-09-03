#include "Device.h"
#include "Base/Log.h"

#include <GLFW/glfw3.h>

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

namespace wind {
static std::vector<const char*> layers = {"VK_LAYER_KHRONOS_validation"};

VKAPI_ATTR VkBool32 VKAPI_CALL DebugUtilsMessengerCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT      messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT             messageTypes,
    VkDebugUtilsMessengerCallbackDataEXT const* pCallbackData, void* /*pUserData*/) {
    WIND_CORE_INFO("validation layer: {}", pCallbackData->pMessage);
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

GPUDevice::GPUDevice() {
    PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr = m_vkLoader.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");
    VULKAN_HPP_DEFAULT_DISPATCHER.init(vkGetInstanceProcAddr);

    vk::ApplicationInfo applicationInfo( "App", 1, "Engine", 1, VK_API_VERSION_1_3 );

    auto extensions = GetRequiredExtensions();
    vk::InstanceCreateInfo instanceCreateInfo( {}, &applicationInfo, uint32_t(layers.size()), layers.data(), (uint32_t)extensions.size(), extensions.data());
    
    m_vkInstance = vk::createInstance(instanceCreateInfo, nullptr);
    VULKAN_HPP_DEFAULT_DISPATCHER.init(m_vkInstance);
    
    // maybe this is right
    m_dubugMessenger = m_vkInstance.createDebugUtilsMessengerEXT(MakeDebugUtilsMessengerCreateInfoEXT(), nullptr);
}

GPUDevice::~GPUDevice() {
    m_vkInstance.destroyDebugUtilsMessengerEXT(m_dubugMessenger);
    m_vkInstance.destroy();
}

}; // namespace wind