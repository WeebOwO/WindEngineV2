#include "Command.h"

namespace wind {
RenderEncoder::RenderEncoder() : CommandEncoder(RenderCommandQueueType::Graphics) {}

void RenderEncoder::BeginRenderPass(const vk::RenderPassBeginInfo& renderPassBeginInfo) {
    m_nativeHandle.beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);
}

void RenderEncoder::EndRenderPass() { m_nativeHandle.endRenderPass(); }

void RenderEncoder::BeginRendering(const vk::RenderingInfo& renderingInfo) {
    m_nativeHandle.beginRendering(renderingInfo);
}

void RenderEncoder::EndRendering() { m_nativeHandle.endRendering(); }
} // namespace wind