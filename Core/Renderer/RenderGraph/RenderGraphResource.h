#pragma once

#include "RenderBackend/Texture.h"
#include "std.h"

#include "RenderBackend/VulkanHeader.h"

namespace wind {
class RenderGraph;

enum RenderGraphResourceType : u8 { Buffer = 0, Texture };

template <RenderGraphResourceType ResourceType> class RenderGraphResource {
public:
    virtual void Init();
    virtual void Destroy();

protected:
    static constexpr RenderGraphResourceType resourceType = ResourceType;
    RenderGraph*                             m_renderGraph;
};

struct RenderGraphTexture : public RenderGraphResource<Texture> {
public:
    enum class TextureType : u8 { Texture2D, Texture2DArray, Texture3D, Texture3DArray, CubeMap };

    struct Desc {
        u32                 width, height;
        u32                 depth;
        vk::Format          format;
        vk::ImageUsageFlags usageFlags;
    };

    RenderGraphTexture(RenderGraph& renderGraph, const Desc& desc);

    void Init() override;
    void Destroy() override;

private:
    Ref<GPUTexture> m_texture;
};

struct RenderGraphBuffer {
public:

private:
};
} // namespace wind