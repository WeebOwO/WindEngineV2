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

struct GPUTexture final : public RHIResource<RHIResourceType::Texture> {
public:
    struct Desc {
        u32                     width;
        u32                     height;
        u32                     depth;
        u32                     mipCount;
        u32                     layerCount;
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

private:
    void CreateImageView(const vk::ImageSubresourceRange& range);

    Desc           m_desc;
    AllocatedImage m_allocatedImage;

    vk::ImageView              m_defaultView;
    std::vector<vk::ImageView> m_cubeMapViews; // only useful when we create cubemap
};

} // namespace wind

namespace wind::utils {
vk::ImageAspectFlags FormatToImageAspect(vk::Format format);
u32                  CalculateImageMipLevelCount(const GPUTexture::Desc& desc);
} // namespace wind::utils
