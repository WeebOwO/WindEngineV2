#pragma once

#include "RenderBackend/RenderResource.h"
#include "VulkanHeader.h"

namespace wind {

struct GPUTexture : public RenderResource<RenderResourceType::Texture> {
public:
    struct TextureDesc {
        uint32_t            width;
        uint32_t            height;
        uint32_t            depth;
        vk::Format          format;
        vk::ImageUsageFlags imageUsage;
    };

    static std::shared_ptr<GPUTexture> Create(const TextureDesc& textureDesc);

    GPUTexture();

    virtual void GenerateMips();
    
private:
    vk::Image     m_vkHandle;

    uint32_t m_mipLevelCount{1};
    uint32_t m_layerCount{1};

    u32        width, height, depth;
    vk::Format m_format{vk::Format::eUndefined};

    VmaAllocation m_allocation = {};
};

class GPUTexture2D : public GPUTexture {
public:

private:
    vk::ImageView m_view;
};

class GPUTextureCube : public GPUTexture {
private:
    std::vector<vk::ImageView> m_cubeViews;
};
} // namespace wind