#pragma once

#include "std.h"

#include "RenderGraphResource.h"
#include "RenderBackend/Texture.h"

namespace wind {
class RenderGraphTexture : public RenderGraphResource {
public:
    struct Desc {
        u32                 width;
        u32                 height;
        u32                 depth = 1;
        vk::ImageUsageFlags usage = vk::ImageUsageFlagBits::eColorAttachment;
    };
    
    struct SubResourceDesc {
        u8 level = 0;
        u8 layer = 0;
    };

    void Create(); 
    void Destory();

    RenderGraphTexture() = default;
    RenderGraphTexture(const Desc& desc) : m_descriptor(desc) {}

private:
    Desc            m_descriptor;
    Ref<GPUTexture> m_texture;
};
} // namespace wind1