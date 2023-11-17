#include "RenderPass.h"

#include "Core/Log.h"
#include "Engine/RuntimeContext.h"
#include "RenderGraph.h"
#include "Renderer/RenderGraph/RenderGraphResource.h"
#include "std.h"

namespace wind {
RenderGraphPass::RenderGraphPass(RenderGraph& renderGraph, const std::string& debugName,
                                 RenderCommandQueueType type)
    : m_renderGraph(renderGraph), m_debugName(debugName), m_queueType(type) {}

void RenderGraphPass::SetRenderExecCallBack(const RenderExecCallBack& callback) {
    m_renderExecCallback = callback;
}
void RenderGraphPass::SetStencilClearCallBack(const StencilClearCallBack& callback) {
    m_stencilCallback = callback;
}
void RenderGraphPass::SetRenderColorClearCallBack(const RenderColorClearCallBack& callback) {
    m_renderColorClearCallBack = callback;
}

RenderGraphPass& RenderGraphPass::DeclareRenderTarget(const std::string&    resourceName,
                                                      const AttachmentInfo& attachmentInfo) {

    return *this;
}

RenderGraphPass& RenderGraphPass::DeclareDepthStencil(const std::string&    resourceName,
                                                      const AttachmentInfo& attachmentInfo) {

    return *this;
}

bool RenderGraphPass::IsWriteToDepth() { return m_writeToDepth; }

bool RenderGraphPass::IsWriteToBackBuffer() { return m_writeToBackBuffer; }

void RenderGraphPass::MarkWriteBackBuffer() { m_writeToBackBuffer = true; }

bool RenderGraphPass::ContainsResource(const std::string& resourceName) {
    return m_renderTargets.contains(resourceName);
}

RenderGraphPass& RenderGraphPass::SetRenderArea(const vk::Rect2D& rect) {
    m_renderArea = rect;
    return *this;
}

void RenderGraphPass::Bake() {
    auto& blackBoard = m_renderGraph.GetBlackBoard();
    auto  device     = RuntimeUtils::GetVulkanDevice();

    
}
} // namespace wind