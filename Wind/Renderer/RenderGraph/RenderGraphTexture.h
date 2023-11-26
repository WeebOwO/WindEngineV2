#pragma once

#include "std.h"

#include "RenderBackend/Texture.h"
#include "RenderGraphResource.h"

namespace wind {
class RenderGraphTexture : public RenderGraphResource {
public:
    struct Desc {
        u32                   width;
        u32                   height;
        u32                   depth   = 1;
        vk::ImageUsageFlags   usage   = vk::ImageUsageFlagBits::eColorAttachment;
    };

    struct SubResourceDesc {
        u8 level = 0;
        u8 layer = 0;
    };

    void Create();
    void Destory();

    auto GetDesc() { return m_descriptor; }
    auto GetNativeHandle() {}
    RenderGraphTexture() = default;
    RenderGraphTexture(const Desc& desc) : m_descriptor(desc) {}

    auto GetCurrentLayout() const { return m_currentLayout; }
    auto GetImageView() const { return m_texture->GetView(); }

private:
    friend class RenderPassNode;

    vk::ImageLayout m_currentLayout;
    Desc            m_descriptor;
    Ref<GPUTexture> m_texture;
};
} // namespace wind