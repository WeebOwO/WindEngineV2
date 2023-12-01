#include "Texture.h"

#include "Device.h"
#include "Engine/RuntimeContext.h"

namespace wind {
static vk::ImageType GetImageType(TextureViewType viewType) {
    if (viewType == TextureViewType::Texture2D || viewType == TextureViewType::Texture2DArray ||
        viewType == TextureViewType::CubeMap) {
        return vk::ImageType::e2D;
    }
    if (viewType == TextureViewType::Texture3D) { return vk::ImageType::e3D; }
    return vk::ImageType::e1D;
};

static vk::ImageViewType GetImageViewType(TextureViewType viewtype) {
    switch (viewtype) {
    case TextureViewType::Texture1D:
        return vk::ImageViewType::e1D;
    case TextureViewType::Texture2D:
        return vk::ImageViewType::e2D;
        break;
    case TextureViewType::Texture2DArray:
        return vk::ImageViewType::e2DArray;
        break;
    case TextureViewType::Texture3D:
        return vk::ImageViewType::e3D;
        break;
    case TextureViewType::CubeMap:
        return vk::ImageViewType::eCube;
    case TextureViewType::CubeMapArray:
        return vk::ImageViewType::eCubeArray;
    }
}

static vk::ImageSubresourceRange GetDefaultImageSubresourceRange(const GPUTexture::Desc& desc) {
    return vk::ImageSubresourceRange{.aspectMask     = utils::FormatToImageAspect(desc.format),
                                     .baseMipLevel   = 0,
                                     .levelCount     = desc.mipCount,
                                     .baseArrayLayer = 0,
                                     .layerCount     = desc.layerCount};
}

void GPUTexture::CreateImageView(const vk::ImageSubresourceRange& range) {
    vk::ImageViewCreateInfo viewCreateInfo{.image            = m_allocatedImage.image,
                                           .viewType         = GetImageViewType(m_desc.viewType),
                                           .format           = m_desc.format,
                                           .subresourceRange = range};

    m_defaultView = device.GetVkDeviceHandle().createImageView(viewCreateInfo);
}

GPUTexture::GPUTexture(const Desc& desc) : m_desc(desc) {
    vk::Extent3D extent3D{.width = desc.width, .height = desc.height, .depth = desc.depth};

    vk::ImageCreateInfo imageCreateInfo{.imageType   = GetImageType(desc.viewType),
                                        .format      = desc.format,
                                        .extent      = extent3D,
                                        .mipLevels   = desc.mipCount,
                                        .arrayLayers = 1,
                                        .samples     = desc.sampleCount,
                                        .usage       = desc.usage};

    VmaAllocationCreateInfo allocationInfo{.flags    = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT,
                                           .usage    = VMA_MEMORY_USAGE_AUTO,
                                           .priority = 1.0f};

    m_allocatedImage = device.AllocateImage(imageCreateInfo, allocationInfo);
    CreateImageView(GetDefaultImageSubresourceRange(desc));
}

Ref<GPUTexture> GPUTexture::Create(const Desc& desc) { return ref::Create<GPUTexture>(desc); }

GPUTexture::~GPUTexture() {
    auto& device = g_runtimeContext.device;
    device->GetVkDeviceHandle().destroyImageView(m_defaultView);
    // use device allocator to destroy
    device->DestroyImage(m_allocatedImage);
}
} // namespace wind

namespace wind::utils {
u32 CalculateImageMipLevelCount(const GPUTexture::Desc& desc) {
    return (uint32_t)std::floor(std::log2(std::max(desc.width, desc.height))) + 1;
}

vk::ImageAspectFlags FormatToImageAspect(vk::Format format) {
    switch (format) {
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
} // namespace wind::utils