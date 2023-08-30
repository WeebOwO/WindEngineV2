module;

#include <vulkan/vulkan_core.h>

module BackendVK;


namespace wind {
GPUDevice::GPUDevice() {
    // create vk Instance
    // vk::ApplicationInfo    appInfo{"WindEngineV2", 1, "WindEngine", VK_API_VERSION_1_3};
    // vk::InstanceCreateInfo instanceCreateInfo {vk::InstanceCreateFlags{}, &appInfo};
    
    // m_instance =  vk::createInstance(instanceCreateInfo);
}

GPUDevice::~GPUDevice() {
    // m_instance.destroy();
}
}
