#include "Texture.h"

#include "Device.h"
#include "Utils.h"

namespace wind
{
    static bool IsIntegerBasedFormat(vk::Format format) { return false; };

    void GPUTexture::CreateImageView(const vk::ImageSubresourceRange& range, vk::ImageViewType viewType)
    {
        vk::ImageViewCreateInfo viewCreateInfo {.image            = m_allocatedImage.image,
                                                .viewType         = viewType,
                                                .format           = m_desc.format,
                                                .subresourceRange = range};

        m_defaultView = device.GetVkDeviceHandle().createImageView(viewCreateInfo);
    }

    // Get ImageSubresourceRange interface
    vk::ImageSubresourceRange GPUTexture::GetDefaultImageSubresourceRange() const
    {
        return GetImageSubresourceRange(0, 0);
    }

    vk::ImageSubresourceRange GPUTexture::GetImageSubresourceRange(uint32_t mip, uint32_t level) const
    {
        return vk::ImageSubresourceRange {.aspectMask     = utils::ImageFormatToImageAspect(m_desc.format),
                                          .baseMipLevel   = mip,
                                          .levelCount     = m_desc.mipCount,
                                          .baseArrayLayer = level,
                                          .layerCount     = m_desc.layerCount};
    }

    GPUTexture::GPUTexture(const vk::ImageCreateInfo& createInfo)
    {
        // init our GPUTextureDesc 
        m_desc = Desc {
            .width = createInfo.extent.width,
            .height = createInfo.extent.height,
            .depth = createInfo.extent.depth,
            .mipCount = createInfo.mipLevels,
            .layerCount = createInfo.arrayLayers,
            .format = createInfo.format,
            .usage = createInfo.usage,
            .sampleCount = createInfo.samples,
            .layout = createInfo.initialLayout
        };

        VmaAllocationCreateInfo allocationInfo {
            .flags = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT, .usage = VMA_MEMORY_USAGE_AUTO, .priority = 1.0f};

        m_allocatedImage = device.AllocateImage(createInfo, allocationInfo);
        m_defaultSampler = utils::CreateDefaultSampler();
    }

    void GPUTexture::MarkUseByImgui(const ImVec2& size,
                                    const ImVec2& uv0,
                                    const ImVec2& uv1,
                                    const ImVec4& tint_col,
                                    const ImVec4& border_colc)
    {
        if (!m_imguiSet) 
            m_imguiSet = ImGui_ImplVulkan_AddTexture(m_defaultSampler, m_defaultView, (VkImageLayout)m_desc.layout);
        ImGui::Image((ImTextureID)m_imguiSet, size, uv0, uv1, tint_col, border_colc);
    }

    Ref<GPUTexture> GPUTexture::Create(const vk::ImageCreateInfo& createInfo) { return ref::Create<GPUTexture>(createInfo); }

    GPUTexture::~GPUTexture()
    {
        auto& device = g_runtimeContext.device;
        if (m_defaultSampler)
            device->GetVkDeviceHandle().destroySampler(m_defaultSampler);
        device->GetVkDeviceHandle().destroyImageView(m_defaultView);
        // use device allocator to destroy
        device->DestroyImage(m_allocatedImage);
    }
} // namespace wind

namespace wind::utils
{
    uint32_t CalculateImageMipLevelCount(const GPUTexture::Desc& desc)
    {
        return (uint32_t)std::floor(std::log2(std::max(desc.width, desc.height))) + 1;
    }

    vk::ImageAspectFlags ImageFormatToImageAspect(vk::Format format)
    {
        switch (format)
        {
            case vk::Format::eD16Unorm:
                return vk::ImageAspectFlagBits::eDepth;
            case vk::Format::eX8D24UnormPack32:
                return vk::ImageAspectFlagBits::eDepth;
            case vk::Format::eD32Sfloat:
                return vk::ImageAspectFlagBits::eDepth;
            case vk::Format::eD16UnormS8Uint:
                return vk::ImageAspectFlagBits::eDepth | vk::ImageAspectFlagBits::eStencil;
            case vk::Format::eD24UnormS8Uint:
                return vk::ImageAspectFlagBits::eDepth | vk::ImageAspectFlagBits::eStencil;
            case vk::Format::eD32SfloatS8Uint:
                return vk::ImageAspectFlagBits::eDepth | vk::ImageAspectFlagBits::eStencil;
            default:
                return vk::ImageAspectFlagBits::eColor;
        }
    }

    vk::ImageLayout ImageUsageToImageLayout(vk::ImageUsageFlagBits usage)
    {
        using namespace vk;

        switch (usage)
        {
            case ImageUsageFlagBits::eTransferSrc:
                return ImageLayout::eTransferSrcOptimal;
            case ImageUsageFlagBits::eTransferDst:
                return ImageLayout::eTransferDstOptimal;
            case ImageUsageFlagBits::eSampled:
                return ImageLayout::eShaderReadOnlyOptimal;
            case ImageUsageFlagBits::eColorAttachment:
                return ImageLayout::eColorAttachmentOptimal;
            case ImageUsageFlagBits::eDepthStencilAttachment:
                return ImageLayout::eDepthStencilAttachmentOptimal;
            case ImageUsageFlagBits::eStorage:
                return ImageLayout::eGeneral;
        }

        return ImageLayout::eUndefined;
    }

    vk::AccessFlags ImageUsageToAccessFlags(vk::ImageUsageFlagBits usage)
    {
        using namespace vk;
        switch (usage)
        {
            case ImageUsageFlagBits::eTransferSrc:
                return AccessFlagBits::eTransferRead;
            case ImageUsageFlagBits::eTransferDst:
                return AccessFlagBits::eTransferWrite;
            case ImageUsageFlagBits::eSampled:
                return AccessFlagBits::eShaderRead;
            case ImageUsageFlagBits::eStorage:
                return AccessFlagBits::eShaderRead | AccessFlagBits::eShaderWrite;
            case ImageUsageFlagBits::eColorAttachment:
                return AccessFlagBits::eColorAttachmentWrite;
            case ImageUsageFlagBits::eDepthStencilAttachment:
                return AccessFlagBits::eDepthStencilAttachmentWrite;
            case ImageUsageFlagBits::eInputAttachment:
                return vk::AccessFlagBits::eInputAttachmentRead;
            case ImageUsageFlagBits::eFragmentShadingRateAttachmentKHR:
                return vk::AccessFlagBits::eFragmentShadingRateAttachmentReadKHR;
        }
        return {};
    }
} // namespace wind::utils