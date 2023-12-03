#pragma once

#include "std.h"

#include "RenderBackend/Texture.h"
#include "RenderGraphResource.h"

namespace wind {
class RenderGraphTexture : public RenderGraphResource {
public:
    struct Desc {
        u32                 width;
        u32                 height;
        u32                 depth;
        vk::Format          format;
        vk::ImageLayout     layout    = vk::ImageLayout::eUndefined;
        vk::ImageUsageFlags usage     = vk::ImageUsageFlagBits::eColorAttachment;
        bool                useMipmap = false;
    };

    struct SubResourceDesc {
        u8 level = 0;
        u8 layer = 0;
    };

    auto GetDesc() { return m_desc; }

    RenderGraphTexture() = default;
    RenderGraphTexture(const Desc& desc);

    vk::Image     GetImage() const;
    vk::ImageView GetImageView() const;

    auto GetUsage() const { return m_desc.usage; }
    auto GetLayout() const { return m_desc.layout; }

    void InitRHI() override;
    void ReleaseRHI() override;

    void SetTexture(Ref<GPUTexture> texture);

private:
    friend class RenderPassNode;
    Desc            m_desc;
    Ref<GPUTexture> m_texture;
};
} // namespace wind

namespace wind::utils {
RenderGraphTexture::Desc GetRenderTargetDesc(u32 width, u32 height, vk::Format format,
                                             bool useMipmap = false);
}