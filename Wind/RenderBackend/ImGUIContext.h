#pragma once

#include "Engine/Window.h"
#include "VulkanHeader.h"

namespace wind {

class GPUDevice;

// this part use c-interface vulkan
class ImGUIContext {
public:
    void Init(const GPUDevice& device, const Window& window);
    void Quit(const GPUDevice& device);

private:
    VkDescriptorPool m_imguiPool;
    VkRenderPass     m_renderPass;
};
} // namespace wind