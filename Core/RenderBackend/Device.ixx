export module BackendVK:Device;

import <vector>;

#include <vulkan/vulkan.hpp>

namespace wind {
export class GPUDevice {
public:
    GPUDevice();
    ~GPUDevice();
private:
    vk::Instance m_instance;
    vk::Device m_device;
    vk::PhysicalDevice m_physicalDevice;
};
}

