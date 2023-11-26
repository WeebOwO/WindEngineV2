#pragma once

#include "std.h"

#include "RenderGraphID.h"
#include "RenderGraphPass.h"
#include "RenderGraphTexture.h"

namespace wind {
class ResourceNode;
class RenderGraph;

class PassNode {
public:
    PassNode(RenderGraph& rg) : renderGraph(rg) {}
    virtual void Execute() noexcept {}

protected:
    std::unordered_set<std::string> declareResources;
    RenderGraph&                    renderGraph;
};

class RenderPassNode : public PassNode {
public:
    struct Attachments {
        static constexpr size_t MAX_ATTACHMENT_COUNT = 10;
        union {
            RenderGraphID<RenderGraphTexture> array[MAX_ATTACHMENT_COUNT];
            struct {
                RenderGraphID<RenderGraphTexture> color[MAX_ATTACHMENT_COUNT - 2];
                RenderGraphID<RenderGraphTexture> depth;
                RenderGraphID<RenderGraphTexture> stencil;
            };
        };
    };

    struct Desc {
        Attachments           attchments;
        vk::Viewport          viewPort;
        u8                    sample;
        vk::ClearValue        clearValue;
        vk::AttachmentLoadOp  loadop  = vk::AttachmentLoadOp::eLoad;
        vk::AttachmentStoreOp storeop = vk::AttachmentStoreOp::eStore;
    };

    RenderPassNode(RenderGraph& rg, const std::string& name, Scope<RenderGraphPassBase> pass);

    void DeclareRenderTarget(const Desc& desc);

private:
    std::vector<vk::RenderingAttachmentInfo> m_colorAttachmentInfos;
    vk::RenderingAttachmentInfo              m_depthAttachmentInfo;
    vk::RenderingAttachmentInfo              m_stencilAttachmentInfo;

    vk::RenderingInfo          m_renderingInfo;
    std::string                m_debugName;
    Scope<RenderGraphPassBase> m_passBase;
};
} // namespace wind