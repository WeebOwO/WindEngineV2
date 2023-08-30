#include "Device.h"

#include <GLFW/glfw3.h>

import <vector>;

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
    vk::ApplicationInfo appInfo {"WindEngine", 1, "Wind", VK_API_VERSION_1_3};
    vk::InstanceCreateInfo instanceCreateInfo{{}, &appInfo};

    m_instance = vk::createInstance(instanceCreateInfo);

    m_physicalDevice = m_instance.enumeratePhysicalDevices().front();
}


}; // namespace wind