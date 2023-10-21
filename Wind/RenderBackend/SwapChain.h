#pragma once

#include "VulkanHeader.h"

#include "Device.h"
#include "Texture.h"

namespace wind {

class Window;
class Swapchain {
public:
    static constexpr uint32_t MAX_FRAME_IN_FLIGHT = 2;

    Swapchain(const GPUDevice& device, const Window& window);
    ~Swapchain();

    void Resize(u32 width, u32 height);
    u32  ImageCount() const { return m_swapchainImages.size(); }
    auto GetFrameBuffer(u32 index) const { return m_framebuffers[index]; }
    auto GetImageView(u32 index) const { return m_swapchainViews[index]; }
    auto GetClearValue() const { return &m_clearValue; }

    std::pair<u32, u32> GetWindowExtent() const {
        return {m_windowExtent.width, m_windowExtent.height};
    }
    u32   GetWidth() const { return m_windowExtent.width; }
    u32   GetHeight() const { return m_windowExtent.height; }
    float GetAspectRatio() const {
        return static_cast<float>(m_windowExtent.width) / static_cast<float>(m_windowExtent.height);
    }

    auto GetRenderPass() const { return m_renderPass; }
    void SetFrameNumber(u32 currentFrame) { m_frameNumber = currentFrame; }
    void SetClearColor(float r, float g, float b, float a);

    std::optional<u32> AcquireNextImage();
    void               SubmitCommandBuffer(const vk::CommandBuffer& cmdBuffer, u32 imageIndex) const ;

private:
    void QuerySurfaceProperty();
    void CreateSyncObject();
    void GetSwapChainImage();
    void CreateSwapChainInteral(u32 width, u32 height);
    void CleanUpSwapChain();
    void CreateRenderPass();

    void ResetClearValue();
    const GPUDevice& m_device;

    std::vector<vk::Image>     m_swapchainImages;
    std::vector<vk::ImageView> m_swapchainViews;

    std::vector<vk::Semaphore> m_imageAvailableSemaphores;
    std::vector<vk::Semaphore> m_renderFinishedSemaphores;
    std::vector<vk::Fence>     m_fences;

    vk::SwapchainKHR m_swapchain;
    vk::SurfaceKHR   m_surface;

    vk::RenderPass m_renderPass;

    std::vector<vk::Framebuffer> m_framebuffers;

    vk::Extent2D m_windowExtent;

    vk::SurfaceFormatKHR m_surfaceFormat;
    vk::PresentModeKHR   m_surfacePresentMode;

    vk::ClearColorValue m_swapchainClearColor{std::array<float, 4>{0.3f, 0.3f, 0.3f, 0.3f}};
    vk::ClearValue      m_clearValue {};
    bool                m_vsync{true};
    u32                 m_frameNumber;
};

}; // namespace wind