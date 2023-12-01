#include "RenderGraphTexture.h"
#include "RenderBackend/Texture.h"

namespace wind {
vk::Image     RenderGraphTexture::GetImage() const { return m_texture->GetVkImage(); }
vk::ImageView RenderGraphTexture::GetImageView() const { return m_texture->GetView(); }

RenderGraphTexture::RenderGraphTexture(const Desc& desc) : m_desc(desc) {
    InitRHI();
}

void RenderGraphTexture::SetTexture(Ref<GPUTexture> texture) { m_texture = texture; }

void RenderGraphTexture::InitRHI() {
    GPUTexture::Desc desc{.width      = m_desc.width,
                          .height     = m_desc.height,
                          .depth      = m_desc.depth,
                          .mipCount   = 1,
                          .layerCount = 1,
                          .viewType   = TextureViewType::Texture2D,
                          .format     = m_desc.format,
                          .usage      = m_desc.usage};
    if(m_desc.useMipmap) {
        desc.mipCount = utils::CalculateImageMipLevelCount(desc);
    }
    m_texture = GPUTexture::Create(desc);
}

void RenderGraphTexture::ReleaseRHI() { m_texture.reset(); }
} // namespace wind


namespace wind::utils {
    RenderGraphTexture::Desc GetRenderTargetDesc(u32 width, u32 height, vk::Format format, bool useMipmap) {
        return RenderGraphTexture::Desc {
            .width = width,
            .height = height,
            .depth = 1,
            .format = format,
            .usage = vk::ImageUsageFlagBits::eColorAttachment,
            .useMipmap = useMipmap
        };
    }
}