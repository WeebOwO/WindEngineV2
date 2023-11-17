#include "Texture.h"

#include "Device.h"
#include "Engine/RuntimeContext.h"

namespace wind {
static vk::ImageType GetImageType(TextureViewType viewType) {
    if (viewType == TextureViewType::Texture2D || viewType == TextureViewType::Texture2DArray ||
        viewType == TextureViewType::CubeMap) {
        return vk::ImageType::e2D;
    }
    if (viewType == TextureViewType::Texture3D || viewType == TextureViewType::Texture3DArray) {
        return vk::ImageType::e3D;
    }
    return vk::ImageType::e1D;
};

static u32 CalculateImageMipLevelCount(const GPUTexture::Desc& desc) {
    if (desc.generateMipMap)
        return (uint32_t)std::floor(std::log2(std::max(desc.width, desc.height))) + 1;
    else
        return 1;
}

GPUTexture::GPUTexture(const vk::ImageCreateInfo&     imageCreateInfo,
                       const VmaAllocationCreateInfo& vmaCreateInfo) {
    m_allocatedImage = device.AllocateImage(imageCreateInfo, vmaCreateInfo);
}

Ref<GPUTexture> GPUTexture::Create(const Desc& desc) {
    vk::Extent3D extent3D{.width = desc.width, .height = desc.height, .depth = desc.depth};

    vk::ImageCreateInfo imageCreateInfo{.imageType   = GetImageType(desc.viewType),
                                        .format      = desc.format,
                                        .extent      = extent3D,
                                        .mipLevels   = CalculateImageMipLevelCount(desc),
                                        .arrayLayers = 1,
                                        .samples     = desc.sampleCount,
                                        .usage       = desc.usage};

    VmaAllocationCreateInfo allocationInfo{.flags    = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT,
                                           .usage    = VMA_MEMORY_USAGE_AUTO,
                                           .priority = 1.0f};

    auto texture = ref::Create<GPUTexture>(imageCreateInfo, allocationInfo);
    texture->SetDesc(desc);

    return texture;
}

GPUTexture::~GPUTexture() {
    auto& device = g_runtimeContext.device;
    device->DestroyImage(m_allocatedImage);
}
} // namespace wind