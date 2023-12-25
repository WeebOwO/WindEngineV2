#pragma once

#include "Allocator.h"
#include "RHIResource.h"
#include "VulkanHeader.h"

namespace wind {

enum class TextureViewType {
    Texture1D,
    Texture2D,
    Texture2DArray,
    Texture3D,
    CubeMap,
    CubeMapArray
};

struct GPUTexture : public RHIResource<RHIResourceType::Texture> {
public:
    struct Desc {
        uint32_t                     width;
        uint32_t                     height;
        uint32_t                     depth;
        uint32_t                     mipCount;
        uint32_t                     layerCount;
        TextureViewType         viewType;
        vk::Format              format;
        vk::ImageUsageFlags     usage;
        vk::SampleCountFlagBits sampleCount = vk::SampleCountFlagBits::e1;
        vk::ImageLayout         layout      = vk::ImageLayout::eUndefined;
    };

    GPUTexture() = default;
    GPUTexture(const Desc& desc);
    ~GPUTexture();

    static Ref<GPUTexture> Create(const Desc& desc);

    auto GetVkImage() const { return m_allocatedImage.image; }
    auto GetView() const { return m_defaultView; }
    auto GetDesc() const { return m_desc; }

    operator vk::Image() { return m_allocatedImage.image; }

private:
    void CreateImageView(const vk::ImageSubresourceRange& range);

    Desc           m_desc;
    AllocatedImage m_allocatedImage;

    vk::ImageView              m_defaultView;
    std::vector<vk::ImageView> m_cubeMapViews; // only useful when we create cubemap
};
} // namespace wind

namespace wind::utils {
// human driver part, we deduce the result from image usage and format
vk::ImageAspectFlags ImageFormatToImageAspect(vk::Format format);
vk::ImageLayout      ImageUsageToImageLayout(vk::ImageUsageFlagBits usage);
uint32_t                  CalculateImageMipLevelCount(const GPUTexture::Desc& desc);
} // namespace wind::utils
