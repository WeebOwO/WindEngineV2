#include "PassNode.h"

#include "RenderGraph.h"

namespace wind {

RenderPassNode::RenderPassNode(RenderGraph& rg, const std::string& name,
                               Scope<RenderGraphPassBase> pass)
    : PassNode(rg), m_debugName(name), m_passBase(std::move(pass)) {}

void RenderPassNode::DeclareRenderTarget(const Desc& desc) {
    bool writeDepth = false, writeStencil = false;
    for (auto attachment : desc.attchments.color) {
        if (attachment) {
            auto                        texture = renderGraph.Get(attachment);
            vk::RenderingAttachmentInfo attachmentInfo{.imageView   = texture->GetImageView(),
                                                       .imageLayout = texture->GetCurrentLayout(),
                                                       .loadOp      = desc.loadop,
                                                       .storeOp     = desc.storeop,
                                                       .clearValue  = desc.clearValue};
            m_colorAttachmentInfos.push_back(attachmentInfo);
        }
    }

    if (desc.attchments.depth) {
        auto texture = renderGraph.Get(desc.attchments.depth);
        m_depthAttachmentInfo =
            vk::RenderingAttachmentInfo{.imageView   = texture->GetImageView(),
                                        .imageLayout = texture->GetCurrentLayout(),
                                        .loadOp      = desc.loadop,
                                        .storeOp     = desc.storeop,
                                        .clearValue  = desc.clearValue};
        writeDepth = true;
    }

    if (desc.attchments.stencil) {
        auto texture = renderGraph.Get(desc.attchments.depth);
        m_stencilAttachmentInfo =
            vk::RenderingAttachmentInfo{.imageView   = texture->GetImageView(),
                                        .imageLayout = texture->GetCurrentLayout(),
                                        .loadOp      = desc.loadop,
                                        .storeOp     = desc.storeop,
                                        .clearValue  = desc.clearValue};
        writeStencil = true;
    }

    m_renderingInfo =
        vk::RenderingInfo{.renderArea           = {.offset = {0, 0},
                                                   .extent = {.width  = (u32)desc.viewPort.width,
                                                              .height = (u32)desc.viewPort.height}},
                          .layerCount           = 1,
                          .colorAttachmentCount = (u32)m_colorAttachmentInfos.size(),
                          .pColorAttachments    = m_colorAttachmentInfos.data(),
                          .pDepthAttachment     = &m_depthAttachmentInfo,
                          .pStencilAttachment   = &m_stencilAttachmentInfo};
}
} // namespace wind