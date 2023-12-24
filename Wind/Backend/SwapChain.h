#pragma once

#include "Device.h"
#include "Texture.h"
#include "VulkanHeader.h"

namespace wind {

class Window;
class Swapchain {
public:
    Swapchain(const GPUDevice& device, const Window& window);
    ~Swapchain();

    void Resize(u32 width, u32 height);
    u32  ImageCount() const { return m_swapchainImages.size(); }
   
    auto GetImageView(u32 index) const { return m_swapchainViews[index]; }
    auto GetImage(u32 index) const { return m_swapchainImages[index]; }
    auto GetFormat() const { return m_surfaceFormat.format; }
    auto GetClearValue() const { return &m_clearValue; }
    auto GetRenderingInfo(u32 index) const { return m_renderingInfos[index]; }

    std::pair<u32, u32> GetWindowExtent() const {
        return {m_windowExtent.width, m_windowExtent.height};
    }
    u32 GetWidth() const { return m_windowExtent.width; }
    u32 GetHeight() const { return m_windowExtent.height; }

    float GetAspectRatio() const {
        return static_cast<float>(m_windowExtent.width) / static_cast<float>(m_windowExtent.height);
    }

    void SetFrameNumber(u32 currentFrame) { m_frameNumber = currentFrame; }
    void SetClearColor(float r, float g, float b, float a);

    std::optional<u32> AcquireNextImage(const vk::Fence&     waitFence,
                                        const vk::Semaphore& imageAvailableSemaphore) const;

    void SubmitCommandBuffer(const vk::CommandBuffer& cmdBuffer, const vk::Fence& signalFence,
                             const vk::Semaphore& imageAvailableSemaphore,
                             const vk::Semaphore& imageFinishSemaphre, u32 imageIndex) const;

private:
    void QuerySurfaceProperty();
    void GetSwapChainImage();
    void CreateSwapChainInteral(u32 width, u32 height);
    void CleanUpSwapChain();
    void CreateRenderPass();

    void             ResetClearValue();
    const GPUDevice& m_device;

    std::vector<vk::Image>     m_swapchainImages;
    std::vector<vk::ImageView> m_swapchainViews;

    vk::SwapchainKHR m_swapchain;
    vk::SurfaceKHR   m_surface;

    vk::Extent2D m_windowExtent;

    vk::SurfaceFormatKHR m_surfaceFormat;
    vk::PresentModeKHR   m_surfacePresentMode;

    vk::ClearColorValue m_swapchainClearColor{std::array<float, 4>{0.3f, 0.3f, 0.3f, 0.3f}};
    vk::ClearValue      m_clearValue{};
    bool                m_vsync{true};

    // test dynamic rendering
    std::vector<vk::RenderingAttachmentInfo> m_attachmentInfos;
    std::vector<vk::RenderingInfo>           m_renderingInfos;
    u32                                      m_frameNumber;
};

}; // namespace wind