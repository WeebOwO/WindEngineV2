#include "SwapChain.h"

#include "Base/Log.h"
#include "Engine/Window.h"

namespace wind {
Swapchain::Swapchain(const GPUDevice& device, const Window& window) : m_device(device) {
    VkSurfaceKHR rawSurface;
    glfwCreateWindowSurface(static_cast<VkInstance>(m_device.GetVkInstance()), window.GetWindow(),
                            nullptr, &rawSurface);
    m_surface = rawSurface;
    CreateSwapChainInteral(window.width(), window.height());
    CreateSyncObject();
    WIND_CORE_INFO("Create swapchain");
}

void Swapchain::QuerySurfaceProperty() {
    auto physicalDevice = m_device.GetVkPhysicalDevice();

    auto presentModes        = physicalDevice.getSurfacePresentModesKHR(m_surface);
    auto surfaceCapabilities = physicalDevice.getSurfaceCapabilitiesKHR(m_surface);
    auto surfaceFormats      = physicalDevice.getSurfaceFormatsKHR(m_surface);

    m_surfacePresentMode = m_vsync ? vk::PresentModeKHR::eFifo : vk::PresentModeKHR::eMailbox;

    m_surfaceFormat = surfaceFormats.front();

    for (const auto& availableFormat : surfaceFormats) {
        if (availableFormat.format == vk::Format::eB8G8R8A8Snorm &&
            availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
            m_surfaceFormat = availableFormat;
            break;
        }
    }
}

Swapchain::~Swapchain() {
    auto vkInstance = m_device.GetVkInstance();
    auto vkDevice   = m_device.GetVkDeviceHandle();
    CleanUpSwapChain();

    for (u32 i = 0; i < MAX_FRAME_IN_FLIGHT; ++i) {
        vkDevice.destroySemaphore(m_imageAvailableSemaphores[i]);
        vkDevice.destroySemaphore(m_renderFinishedSemaphores[i]);
        vkDevice.destroyFence(m_fences[i]);
    }

    vkInstance.destroySurfaceKHR(m_surface);
}

void Swapchain::CreateSwapChainInteral(u32 width, u32 height) {
    auto physicalDevice = m_device.GetVkPhysicalDevice();
    auto vkDevice       = m_device.GetVkDeviceHandle();

    auto surfaceCapabilities = physicalDevice.getSurfaceCapabilitiesKHR(m_surface);

    m_windowExtent = vk::Extent2D(std::clamp(width, surfaceCapabilities.minImageExtent.width,
                                             surfaceCapabilities.maxImageExtent.width),
                                  std::clamp(height, surfaceCapabilities.minImageExtent.height,
                                             surfaceCapabilities.maxImageExtent.height));

    QuerySurfaceProperty();
    vk::SwapchainCreateInfoKHR swapchainCreateInfo{
        .surface          = m_surface,
        .minImageCount    = MAX_FRAME_IN_FLIGHT,
        .imageFormat      = m_surfaceFormat.format,
        .imageColorSpace  = m_surfaceFormat.colorSpace,
        .imageExtent      = m_windowExtent,
        .imageArrayLayers = 1,
        .imageUsage =
            vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransferDst,
        .imageSharingMode = vk::SharingMode::eExclusive,
        .preTransform     = surfaceCapabilities.currentTransform,
        .compositeAlpha   = vk::CompositeAlphaFlagBitsKHR::eOpaque,
        .presentMode      = m_surfacePresentMode,
        .clipped          = true,
        .oldSwapchain     = nullptr};

    m_swapchain = vkDevice.createSwapchainKHR(swapchainCreateInfo);

    m_swapchainImages = vkDevice.getSwapchainImagesKHR(m_swapchain);
    WIND_CORE_INFO("SwapChain image count is {}", m_swapchainImages.size());
    m_swapchainViews.resize(m_swapchainImages.size());

    for (u32 i = 0; i < m_swapchainImages.size(); ++i) {
        vk::ImageSubresourceRange range{.aspectMask     = vk::ImageAspectFlagBits::eColor,
                                        .baseMipLevel   = 0,
                                        .levelCount     = 1,
                                        .baseArrayLayer = 0,
                                        .layerCount     = 1};

        vk::ImageViewCreateInfo viewCreateInfo{.image            = m_swapchainImages[i],
                                               .viewType         = vk::ImageViewType::e2D,
                                               .format           = m_surfaceFormat.format,
                                               .subresourceRange = range};

        m_swapchainViews[i] = vkDevice.createImageView(viewCreateInfo);
    }

    CreateRenderPass();
    ResetClearValue();
}

void Swapchain::CreateRenderPass() {
    auto vkDevice = m_device.GetVkDeviceHandle();

    vk::AttachmentDescription colorAttachment{
        .format         = m_surfaceFormat.format,
        .samples        = vk::SampleCountFlagBits::e1,
        .loadOp         = vk::AttachmentLoadOp::eClear,
        .storeOp        = vk::AttachmentStoreOp::eStore,
        .stencilLoadOp  = vk::AttachmentLoadOp::eDontCare,
        .stencilStoreOp = vk::AttachmentStoreOp::eDontCare,
        .initialLayout  = vk::ImageLayout::eUndefined,
        .finalLayout    = vk::ImageLayout::ePresentSrcKHR,
    };

    vk::AttachmentReference colorReference{.attachment = 0,
                                           .layout     = vk::ImageLayout::eAttachmentOptimal};

    vk::SubpassDescription subpassDesc{
        .pipelineBindPoint    = vk::PipelineBindPoint::eGraphics,
        .colorAttachmentCount = 1,
        .pColorAttachments    = &colorReference,
    };

    vk::SubpassDependency denpendency{
        .srcSubpass    = VK_SUBPASS_EXTERNAL,
        .dstSubpass    = 0,
        .srcStageMask  = vk::PipelineStageFlagBits::eColorAttachmentOutput,
        .dstStageMask  = vk::PipelineStageFlagBits::eColorAttachmentOutput,
        .srcAccessMask = vk::AccessFlagBits::eColorAttachmentWrite,
        .dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite};

    vk::RenderPassCreateInfo renderPassCreateInfo{.attachmentCount = 1,
                                                  .pAttachments    = &colorAttachment,
                                                  .subpassCount    = 1,
                                                  .pSubpasses      = &subpassDesc,
                                                  .dependencyCount = 1,
                                                  .pDependencies   = &denpendency};

    m_renderPass = vkDevice.createRenderPass(renderPassCreateInfo);

    // create framebuffer
    m_framebuffers.resize(m_swapchainImages.size());

    for (u32 i = 0; i < m_framebuffers.size(); ++i) {
        std::vector<vk::ImageView> attachments = {m_swapchainViews[i]};
        vk::FramebufferCreateInfo  frameBufferCreateInfo{.renderPass      = m_renderPass,
                                                         .attachmentCount = (u32)attachments.size(),
                                                         .pAttachments    = attachments.data(),
                                                         .width           = m_windowExtent.width,
                                                         .height          = m_windowExtent.height,
                                                         .layers          = 1};

        m_framebuffers[i] = vkDevice.createFramebuffer(frameBufferCreateInfo);
    }
}

void Swapchain::CreateSyncObject() {
    auto vkDevice = m_device.GetVkDeviceHandle();

    m_imageAvailableSemaphores.resize(MAX_FRAME_IN_FLIGHT);
    m_renderFinishedSemaphores.resize(MAX_FRAME_IN_FLIGHT);
    m_fences.resize(MAX_FRAME_IN_FLIGHT);

    vk::FenceCreateInfo fenceCreateInfo{.flags = vk::FenceCreateFlagBits::eSignaled};

    for (u32 i = 0; i < MAX_FRAME_IN_FLIGHT; ++i) {
        m_imageAvailableSemaphores[i] = vkDevice.createSemaphore({});
        m_renderFinishedSemaphores[i] = vkDevice.createSemaphore({});
        m_fences[i]                   = vkDevice.createFence(fenceCreateInfo);
    }
}

std::optional<u32> Swapchain::AcquireNextImage() {
    auto vkDevice = m_device.GetVkDeviceHandle();
    auto _ =
        vkDevice.waitForFences(m_fences[m_frameNumber], true, std::numeric_limits<uint64_t>::max());

    if (_ != vk::Result::eSuccess) { WIND_CORE_ERROR("Fail to get next image index"); }

    vkDevice.resetFences(m_fences[m_frameNumber]);

    auto result = vkDevice.acquireNextImageKHR(m_swapchain, std::numeric_limits<uint64_t>::max(),
                                               m_imageAvailableSemaphores[m_frameNumber]);

    if (result.result != vk::Result::eSuccess) { return std::nullopt; }
    return result.value;
}

void Swapchain::SubmitCommandBuffer(const vk::CommandBuffer& cmdBuffer, u32 imageIndex) const {
    std::vector<vk::PipelineStageFlags> waitStage{
        vk::PipelineStageFlagBits::eColorAttachmentOutput};

    vk::SubmitInfo submitInfo{.waitSemaphoreCount   = 1,
                              .pWaitSemaphores      = &m_imageAvailableSemaphores[m_frameNumber],
                              .pWaitDstStageMask    = waitStage.data(),
                              .commandBufferCount   = 1,
                              .pCommandBuffers      = &cmdBuffer,
                              .signalSemaphoreCount = 1,
                              .pSignalSemaphores    = &m_renderFinishedSemaphores[m_frameNumber]};

    m_device.GetGraphicsQueue().submit(submitInfo, m_fences[m_frameNumber]);

    vk::PresentInfoKHR presentInfo{.waitSemaphoreCount = 1,
                                   .pWaitSemaphores    = &m_renderFinishedSemaphores[m_frameNumber],
                                   .swapchainCount     = 1,
                                   .pSwapchains        = &m_swapchain,
                                   .pImageIndices      = &imageIndex};

    auto presentResult = m_device.GetGraphicsQueue().presentKHR(presentInfo);
}

void Swapchain::CleanUpSwapChain() {
    auto vkdevice = m_device.GetVkDeviceHandle();

    for (auto& view : m_swapchainViews) {
        vkdevice.destroyImageView(view);
    }

    for (auto& framebuffer : m_framebuffers) {
        vkdevice.destroyFramebuffer(framebuffer);
    }

    vkdevice.destroyRenderPass(m_renderPass);
    vkdevice.destroySwapchainKHR(m_swapchain);
}

void Swapchain::SetClearColor(float r, float g, float b, float a) {
    m_swapchainClearColor.setFloat32(std::array<float, 4>{r, g, b, a});
    ResetClearValue();
}

void Swapchain::ResetClearValue() {
    m_clearValue.setColor(m_swapchainClearColor);
}
} // namespace wind