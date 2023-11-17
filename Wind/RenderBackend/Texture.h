#pragma once

#include "Allocator.h"
#include "RHIResource.h"
#include "VulkanHeader.h"

namespace wind {

enum class TextureViewType { Texture2D, Texture2DArray, Texture3D, Texture3DArray, CubeMap };

struct GPUTexture final : public RHIResource<RHIResourceType::Texture> {
public:
    struct Desc {
        u32                     width;
        u32                     height;
        u32                     depth;
        vk::Format              format;
        TextureViewType         viewType;
        vk::ImageUsageFlags     usage;
        vk::SampleCountFlagBits sampleCount = vk::SampleCountFlagBits::e1;
        vk::ImageLayout         layout{vk::ImageLayout::eUndefined};
        bool                    generateMipMap = false;
    };

    GPUTexture() = default;
    ~GPUTexture();

    static Ref<GPUTexture> Create(const Desc& desc);

private:
    GPUTexture(const vk::ImageCreateInfo&     imageCreateInfo,
               const VmaAllocationCreateInfo& vmaCreateInfo);
    void           SetDesc(const Desc& desc) { m_desc = desc; }

    AllocatedImage m_allocatedImage;
    Desc           m_desc;
};
} // namespace wind