#pragma once

#include "VulkanHeader.h"

#include "RenderResource.h"

namespace wind {

struct GPUTexture : public RHIResource<RenderResourceType::Texture> {
public:
    GPUTexture() = default;
    GPUTexture(const vk::ImageCreateInfo&     desc,
               const VmaAllocationCreateInfo& AllocationCreateInfo);

    virtual void GenerateMips();
    virtual void Init();

private:
    Scope<AllocatedImage> m_image;

    uint32_t m_mipLevelCount{1};
    uint32_t m_layerCount{1};

    u32        width, height, depth;
    vk::Format m_format{vk::Format::eUndefined};
};

class GPUTexture2D : public GPUTexture {
public:
    struct Desc {
        u32                 width;
        u32                 height;
        u32                 mipCount;
        vk::Format          format;
        vk::ImageUsageFlags usage;
    };

    GPUTexture2D(const Desc& desc);

private:
    vk::ImageView m_view;
};

class GPUTexture3D : public GPUTexture {
public:
    struct Desc {
        u32                 width;
        u32                 height;
        u32                 depth;
        u32                 mipCount;
        vk::Format          format;
        vk::ImageUsageFlags usage;
    };

    GPUTexture3D(const Desc& desc);

private:
    vk::ImageView m_view;
};

class GPUTextureCube : public GPUTexture {
public:
private:
    std::vector<vk::ImageView> m_cubeViews;
};
} // namespace wind