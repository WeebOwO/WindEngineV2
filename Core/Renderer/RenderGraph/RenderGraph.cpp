#include "RenderGraph.h"

namespace wind {
void RenderGraph::ImportBackBuffer(const std::string&     backBufferName,
                                   const vk::Framebuffer& framebuffer,
                                   const vk::RenderPass&  renderPass) {
    m_backBufferDebugName = backBufferName;
    m_backBuffer          = framebuffer;
    m_presentRenderPass   = renderPass;
}
} // namespace wind