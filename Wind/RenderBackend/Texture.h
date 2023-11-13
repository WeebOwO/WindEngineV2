#pragma once

#include "Allocator.h"
#include "VulkanHeader.h"
#include "RHIResource.h"

namespace wind {

struct GPUTexture : public RHIResource<RHIResourceType::Texture> {
public:
    GPUTexture() = default;

    virtual void GenerateMips();
    virtual void InitView();

protected:
    AllocatedImage allocatedImage;

    uint32_t mipLevelCount{1};
    uint32_t layerCount{1};

    u32        width, height, depth;
    vk::Format format{vk::Format::eUndefined};
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

    Ref<GPUTexture2D> Create(const Desc& desc);

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

private:
    vk::ImageView m_view;
};

class GPUTextureCube : public GPUTexture {
private:
    std::vector<vk::ImageView> m_cubeViews;
};
} // namespace wind