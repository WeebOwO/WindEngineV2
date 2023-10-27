#include "RenderPass.h"

#include "Core/Log.h"

namespace wind {
RenderGraphPass::RenderGraphPass(RenderGraph& renderGraph, const std::string& debugName,
                                 RenderCommandQueueType type)
    : m_renderGraph(renderGraph), m_debugName(debugName), m_passtype(type) {}

void RenderGraphPass::SetRenderExecCallBack(const RenderExecCallBack& callback) {
    m_renderExecCallback = callback;
}
void RenderGraphPass::SetStencilClearCallBack(const StencilClearCallBack& callback) {
    m_stencilCallback = callback;
}
void RenderGraphPass::SetRenderColorClearCallBack(const RenderColorClearCallBack& callback) {
    m_renderColorClearCallBack = callback;
}

RenderGraphPass& RenderGraphPass::AddColorOuput(const std::string&    resourceName,
                                                const AttachmentInfo& attachmentInfo) {
    m_colorAttachmentLUT[resourceName] = attachmentInfo;
    return *this;
}

RenderGraphPass& RenderGraphPass::AddDepthStencilOutput(const std::string&    resourceName,
                                                        const AttachmentInfo& attachmentInfo) {
    if (!m_depthOutput.has_value()) {
        m_depthOutput = std::make_optional<DepthOuput>(resourceName, attachmentInfo);
    } else {
        WIND_CORE_WARN("Pass {} already have a depth output", m_debugName);
    }
    return *this;
}

bool RenderGraphPass::IsWriteToDepth() { return m_writeToDepth; }

bool RenderGraphPass::IsWriteToBackBuffer() { return m_writeToBackBuffer; }

void RenderGraphPass::MarkWriteDepth() { m_writeToDepth = true; }

void RenderGraphPass::MarkWriteBackBuffer() { m_writeToBackBuffer = true; }

bool RenderGraphPass::ContainsResource(const std::string& resourceName) {
    return m_colorAttachmentLUT.contains(resourceName);
}

RenderGraphPass& RenderGraphPass::SetRenderArea(const vk::Rect2D& rect) {
    m_renderArea = rect;
    return *this;
}
} // namespace wind