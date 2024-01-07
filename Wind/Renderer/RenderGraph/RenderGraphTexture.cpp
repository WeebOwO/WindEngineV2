#include "RenderGraphTexture.h"

#include "Backend/Texture.h"

namespace wind
{
    vk::Image     RenderGraphTexture::GetImage() const noexcept{ return m_texture->GetVkImage(); }
    vk::ImageView RenderGraphTexture::GetImageView() const noexcept { return m_texture->GetView(); }

    RenderGraphTexture::RenderGraphTexture(const Desc& desc) noexcept :
        RenderGraphResource(RenderGraphResourceType::Texture), m_desc(desc)
    {}

    void RenderGraphTexture::SetTexture(Ref<GPUTexture> texture) { m_texture = texture; }

    void RenderGraphTexture::InitRHI()
    {
        // todo: if someone call this twice, we need to check the new desc same with old
        GPUTexture::Desc desc {.width      = m_desc.width,
                               .height     = m_desc.height,
                               .depth      = m_desc.depth,
                               .mipCount   = 1,
                               .layerCount = 1,
                               .viewType   = TextureViewType::Texture2D,
                               .format     = m_desc.format,
                               .usage      = m_desc.usage};
        if (m_desc.useMipmap)
        {
            desc.mipCount = utils::CalculateImageMipLevelCount(desc);
        }
        m_texture = GPUTexture::Create(desc);
    }

    RenderGraphTexture::~RenderGraphTexture() { ReleaseRHI(); }

    void RenderGraphTexture::ReleaseRHI() { m_texture.reset(); }

} // namespace wind

namespace wind::utils
{
    RenderGraphTexture::Desc GetRenderTargetDesc(uint32_t width, uint32_t height, vk::Format format, bool useMipmap)
    {
        return RenderGraphTexture::Desc {.width     = width,
                                         .height    = height,
                                         .depth     = 1,
                                         .format    = format,
                                         .layout    = vk::ImageLayout::eColorAttachmentOptimal,
                                         .usage     = vk::ImageUsageFlagBits::eColorAttachment,
                                         .useMipmap = useMipmap};
    }
} // namespace wind::utils