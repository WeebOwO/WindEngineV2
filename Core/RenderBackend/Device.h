#pragma once

#include "VulkanHeader.h"

namespace wind {
class GPUDevice {
public:
    GPUDevice();
    ~GPUDevice();
private:
    std::vector<const char*> GetRequiredExtensions();
    
    vk::Instance m_vkInstance;
    vk::PhysicalDevice m_physicalDevice;
    vk::Device m_device;

    vk::DebugUtilsMessengerEXT m_dubugMessenger;
    vk::DynamicLoader m_vkLoader{};
    std::vector<vk::ExtensionProperties> m_supportedExtensions;
    
    bool m_enableDebug {true};
};
} // namespace wind 