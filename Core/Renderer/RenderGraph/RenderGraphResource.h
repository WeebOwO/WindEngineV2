#pragma once

#include "std.h"

#include "RenderBackend/Buffer.h"
#include "RenderBackend/Texture.h"
#include "RenderBackend/VulkanHeader.h"

namespace wind {
class RenderGraph;

enum RenderGraphResourceType : u8 { Buffer = 0, Texture };

class RenderGraphResource {
public:
    virtual void Init();
    virtual void Destroy();

protected:
    RenderGraph* m_renderGraph;
};

struct RenderGraphTexture : public RenderGraphResource {
public:
    enum class TextureType : u8 { Texture2D, Texture2DArray, Texture3D, Texture3DArray, CubeMap };

    struct Desc {
        u32                 width;
        u32                 height;
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

struct RenderGraphBuffer : public RenderGraphResource {
public: 
private:
    Ref<GPUBuffer> m_buffer;
};
} // namespace wind