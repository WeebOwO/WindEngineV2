#include "Device.h"

#include <GLFW/glfw3.h>

namespace wind {

std::vector<const char*> GetRequiredExtensions() {
    uint32_t     glfwEextensionsCnt = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwEextensionsCnt);
    std::vector<const char*> extensions(glfwEextensionsCnt);
    for (int i = 0; i < glfwEextensionsCnt; ++i) {
        extensions[i] = glfwExtensions[i];
    }
    return extensions;
}

GPUDevice::GPUDevice() {
    vk::ApplicationInfo appInfo {"WindEngine", 1, "Wind", VK_MAKE_VERSION(1, 0, 3)};
    vk::InstanceCreateInfo instanceCreateInfo{{}, &appInfo};

    m_instance = vk::createInstance(instanceCreateInfo);    

    m_physicalDevice = m_instance.enumeratePhysicalDevices().front();
}


}; // namespace wind