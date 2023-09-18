#include "SwapChain.h"

#include <algorithm>

#include "Base/Log.h"
#include "Engine/Window.h"
#include "RenderBackend/SwapChain.h"

namespace wind {
Swapchain::Swapchain(const GPUDevice& device, const Window& window) : m_device(device) {
    VkSurfaceKHR rawSurface;
    glfwCreateWindowSurface(static_cast<VkInstance>(m_device.GetVkInstance()), window.GetWindow(),
                            nullptr, &rawSurface);
    m_surface = rawSurface;

    auto physicalDevice = m_device.GetVkPhysicalDevice();
    auto vkDevice = m_device.GetVkDeviceHandle();

    auto surfaceCapabilities = physicalDevice.getSurfaceCapabilitiesKHR(m_surface);

    m_surfaceExtent =
        vk::Extent2D(std::clamp(window.width(), surfaceCapabilities.minImageExtent.width,
                                surfaceCapabilities.maxImageExtent.width),
                     std::clamp(window.height(), surfaceCapabilities.minImageExtent.height,
                                surfaceCapabilities.maxImageExtent.height));

    vk::SwapchainCreateInfoKHR swapchainCreateInfo;
    QuerySurfaceProperty();

    swapchainCreateInfo.setSurface(m_surface)
        .setMinImageCount(MAX_FRAME_IN_FLIGHT)
        .setImageFormat(m_surfaceFormat.format)
        .setImageColorSpace(m_surfaceFormat.colorSpace)
        .setImageExtent(m_surfaceExtent)
        .setImageArrayLayers(1)
        .setImageUsage(vk::ImageUsageFlagBits::eColorAttachment |
                       vk::ImageUsageFlagBits::eTransferDst)
        .setImageSharingMode(vk::SharingMode::eExclusive)
        .setPreTransform(surfaceCapabilities.currentTransform)
        .setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque)
        .setPresentMode(m_surfacePresentMode)
        .setClipped(true)
        .setOldSwapchain(nullptr);

    m_swapchain = vkDevice.createSwapchainKHR(swapchainCreateInfo);
    WIND_INFO("Create swapchain");
}

void Swapchain::QuerySurfaceProperty() {
    auto physicalDevice      = m_device.GetVkPhysicalDevice();
    auto presentModes        = physicalDevice.getSurfacePresentModesKHR(m_surface);
    auto surfaceCapabilities = physicalDevice.getSurfaceCapabilitiesKHR(m_surface);
    auto surfaceFormats      = physicalDevice.getSurfaceFormatsKHR(m_surface);

    m_surfacePresentMode = m_vsync ? vk::PresentModeKHR::eFifo : vk::PresentModeKHR::eMailbox;

    m_surfaceFormat = surfaceFormats.front();

    for (const auto& availableFormat : surfaceFormats) {
        if (availableFormat.format == vk::Format::eB8G8R8A8Srgb &&
            availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
            m_surfaceFormat = availableFormat;
            break;
        }
    }
}

Swapchain::~Swapchain() {
    auto vkdevice = m_device.GetVkDeviceHandle();
    auto vkInstance = m_device.GetVkInstance();

    vkdevice.destroySwapchainKHR(m_swapchain);
    vkInstance.destroySurfaceKHR(m_surface);
}

} // namespace wind