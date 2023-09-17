#pragma once

#include <vector>

#include "RenderBackend/Device.h"
#include "RenderBackend/Image.h"
#include "VulkanHeader.h"

namespace wind {

class Window;
class Swapchain {
public:
    static constexpr uint32_t MAX_FRAME_IN_FLIGHT = 2;

    Swapchain(GPUDevice* device, Window* window);
    ~Swapchain();
    void Resize();

private:
    void QuerySurfaceProperty();

    GPUDevice*                       m_device;

    std::vector<GPUTexture>          m_swapchainImages;
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