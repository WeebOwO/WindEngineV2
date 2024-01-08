#include "RenderGraphTexture.h"

#include <regex>

#include "Backend/Texture.h"

namespace wind
{
    vk::Image     RenderGraphTexture::GetImage() const noexcept { return m_texture->GetVkImage(); }
    vk::ImageView RenderGraphTexture::GetImageView() const noexcept { return m_texture->GetView(); }

    RenderGraphTexture::RenderGraphTexture(const Desc& desc) noexcept :
        RenderGraphResource(RenderGraphResourceType::Texture), m_desc(desc)
    {}

    void RenderGraphTexture::SetTexture(Ref<GPUTexture> texture) { m_texture = texture; }

    void RenderGraphTexture::InitRHI()
    {
        // todo: if someone call this twice, we need to check the new desc is same with old ? 
        vk::ImageCreateInfo createInfo {
            .imageType     = vk::ImageType::e2D,
            .format        = m_desc.format,
            .extent        = {.width = m_desc.width, .height = m_desc.height, .depth = m_desc.depth},
            .mipLevels     = 1,
            .arrayLayers   = 1,
            .samples       = vk::SampleCountFlagBits::e1,
            .usage         = m_desc.usage,
            .initialLayout = {}};

        m_texture = GPUTexture::Create(createInfo);
        // todo: current only support 2d image, this is suck
        m_texture->CreateImageView(
            vk::ImageSubresourceRange {.aspectMask     = utils::ImageFormatToImageAspect(m_desc.format),
                                       .baseMipLevel   = 0,
                                       .levelCount     = 1,
                                       .baseArrayLayer = 0,
                                       .layerCount     = 1},
            vk::ImageViewType::e2D);
    }

    RenderGraphTexture::~RenderGraphTexture() noexcept { ReleaseRHI(); }

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