#pragma once

#include "VulkanHeader.h"

#include "RenderBackend/Texture.h"
#include "RenderBackend/Device.h"

namespace wind {

class Window;
class Swapchain {
public:
    static constexpr uint32_t MAX_FRAME_IN_FLIGHT = 2;

    Swapchain(const GPUDevice& device, const Window& window);
    
    ~Swapchain();
    void Resize();

private:
    void QuerySurfaceProperty();
    void CreateSyncObject();

    const GPUDevice& m_device;

    std::vector<GPUTexture>    m_swapchainImages;
    std::vector<vk::Semaphore> m_semaphores;
    std::vector<vk::Fence>     m_fences;

    vk::SwapchainKHR m_swapchain;
    vk::SurfaceKHR   m_surface;

    vk::Extent2D         m_surfaceExtent;
    vk::SurfaceFormatKHR m_surfaceFormat;
    vk::PresentModeKHR   m_surfacePresentMode;

    bool m_vsync{true};
};

}; // namespace wind