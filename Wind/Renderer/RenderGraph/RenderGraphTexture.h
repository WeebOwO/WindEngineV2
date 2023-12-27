#pragma once

#include "std.h"

#include "Backend/Texture.h"
#include "RenderGraphResource.h"

namespace wind {
class RenderGraphTexture : public RenderGraphResource {
public:
    struct Desc {
        uint32_t            width;
        uint32_t            height;
        uint32_t            depth;
        vk::Format          format;
        vk::ImageLayout     layout    = vk::ImageLayout::eUndefined;
        vk::ImageUsageFlags usage     = vk::ImageUsageFlagBits::eColorAttachment;
        bool                useMipmap = false;
    };

    struct SubResourceDesc {
        uint8_t level = 0;
        uint8_t layer = 0;
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
RenderGraphTexture::Desc GetRenderTargetDesc(uint32_t width, uint32_t height, vk::Format format,
                                             bool useMipmap = false);
}