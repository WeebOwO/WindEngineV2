#pragma once

#include "Allocator.h"
#include "RHIResource.h"
#include "VulkanHeader.h"

#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_vulkan.h"

namespace wind
{

    enum class TextureViewType
    {
        Texture1D,
        Texture2D,
        Texture2DArray,
        Texture3D,
        CubeMap,
        CubeMapArray
    };

    struct GPUTexture : public RHIResource<RHIResourceType::Texture>
    {
    public:
        struct Desc
        {
            uint32_t                width;
            uint32_t                height;
            uint32_t                depth;
            uint32_t                mipCount;
            uint32_t                layerCount;
            TextureViewType         viewType;
            vk::Format              format;
            vk::ImageUsageFlags     usage;
            vk::SampleCountFlagBits sampleCount = vk::SampleCountFlagBits::e1;
            vk::ImageLayout         layout      = vk::ImageLayout::eUndefined;
        };

        GPUTexture() = default;
        GPUTexture(const Desc& desc);
        ~GPUTexture();

        static Ref<GPUTexture> Create(const Desc& desc);

        vk::Image                 GetVkImage() const { return m_allocatedImage.image; }
        vk::ImageView             GetView() const { return m_defaultView; }
        Desc                      GetDesc() const { return m_desc; }
        vk::Sampler               GetDefaultSampler() const noexcept { return m_defaultSampler; }
        vk::ImageSubresourceRange GetDefaultImageSubresourceRange() const;
        vk::ImageSubresourceRange GetImageSubresourceRange(uint32_t mip, uint32_t level) const;

        void MarkUseByImgui(const ImVec2& size,
                            const ImVec2& uv0,
                            const ImVec2& uv1,
                            const ImVec4& tint_col   = ImVec4(1, 1, 1, 1),
                            const ImVec4& border_col = ImVec4(0, 0, 0, 0));
                            
        operator vk::Image() { return m_allocatedImage.image; }

    private:
        void CreateImageView(const vk::ImageSubresourceRange& range);

        Desc                       m_desc;
        AllocatedImage             m_allocatedImage;
        vk::Sampler                m_defaultSampler;
        vk::ImageView              m_defaultView;
        std::vector<vk::ImageView> m_cubeMapViews; // only useful when we create cubemap
        vk::DescriptorSet          m_imguiSet;
    };
} // namespace wind

namespace wind::utils
{
    // human driver part, we deduce the result from image usage and format
    vk::ImageAspectFlags   ImageFormatToImageAspect(vk::Format format);
    vk::ImageLayout        ImageUsageToImageLayout(vk::ImageUsageFlagBits usage);
    vk::AccessFlags        ImageUsageToAccessFlags(vk::ImageUsageFlagBits usage);
    vk::PipelineStageFlags ImageUsageToPipelineStage(vk::ImageUsageFlagBits usage);

    uint32_t CalculateImageMipLevelCount(const GPUTexture::Desc& desc);
} // namespace wind::utils
