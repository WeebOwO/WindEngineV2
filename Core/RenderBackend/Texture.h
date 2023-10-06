#pragma once

#include "VulkanHeader.h"

#include "RenderBackend/RenderResource.h"

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
    GPUTexture2D(u32 width, u32 height, vk::Format fomat, vk::ImageUsageFlags flags);
    
private:    
    vk::ImageView m_view;
};

class GPUTexture3D : public GPUTexture {

};  

class GPUTextureCube : public GPUTexture {
public:

private:
    std::vector<vk::ImageView> m_cubeViews;
};
} // namespace wind