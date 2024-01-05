#include "PassNode.h"

#include "RenderGraph.h"
#include "ResourceNode.h"
#include "ResourceRegistry.h"

namespace wind {

void PassNode::InitResources()
{
    auto resources = renderGraph.m_resources;
    // init all the resources 
    for(auto handle : outputResources)
    {
        resources[handle]->InitRHI(); 
    }
}
    
RenderPassNode::RenderPassNode(RenderGraph& rg, const std::string& name,
                               Scope<RenderGraphPassBase> pass)
    : PassNode(rg), m_debugName(name), m_passBase(std::move(pass)) {}

void RenderPassNode::DeclareRenderTarget(const RenderDesc& desc) {
    
    for (auto attachment : desc.attchments.color) {
        if (attachment) {
            auto                        texture = renderGraph.Get(attachment);
            vk::RenderingAttachmentInfo attachmentInfo{.imageView   = texture->GetImageView(),
                                                       .imageLayout = texture->GetLayout(),
                                                       .loadOp      = desc.loadop,
                                                       .storeOp     = desc.storeop,
                                                       .clearValue  = desc.clearValue};
            m_colorAttachmentInfos.push_back(attachmentInfo);
        }
    }

    if(!m_colorAttachmentInfos.empty()) {
        m_renderingInfo.setColorAttachments(m_colorAttachmentInfos)
                       .setColorAttachmentCount(m_colorAttachmentInfos.size());
    }

    if (desc.attchments.depth) {
        auto texture          = renderGraph.Get(desc.attchments.depth);
        m_depthAttachmentInfo = vk::RenderingAttachmentInfo{.imageView   = texture->GetImageView(),
                                                            .imageLayout = texture->GetLayout(),
                                                            .loadOp      = desc.loadop,
                                                            .storeOp     = desc.storeop,
                                                            .clearValue  = desc.clearValue};
        m_renderingInfo.setPDepthAttachment(&m_depthAttachmentInfo);
    }

    if (desc.attchments.stencil) {
        auto texture            = renderGraph.Get(desc.attchments.depth);
        m_stencilAttachmentInfo = vk::RenderingAttachmentInfo{.imageView = texture->GetImageView(),
                                                              .imageLayout = texture->GetLayout(),
                                                              .loadOp      = desc.loadop,
                                                              .storeOp     = desc.storeop,
                                                              .clearValue  = desc.clearValue};
        m_renderingInfo.setPStencilAttachment(&m_stencilAttachmentInfo);
    }

    m_renderingInfo.setLayerCount(1)
                   .setRenderArea(desc.renderArea);
}
} // namespace wind