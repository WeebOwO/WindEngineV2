#pragma once

#include <vulkan/vulkan.hpp>

namespace wind {
class GPUDevice {
private:
    GPUDevice();
    ~GPUDevice();
public:
    vk::Instance m_instance;
    vk::PhysicalDevice m_physicalDevice;
    vk::Device m_device;
};
} // namespace wind 