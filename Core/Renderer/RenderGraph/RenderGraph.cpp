#include "RenderGraph.h"

#include "Base/Log.h"
#include "RenderBackend/SwapChain.h"
#include "Renderer/RenderGraph/RenderPass.h"

namespace wind {
void RenderGraph::ImportBackBuffer(const std::string&     backBufferName,
                                   const vk::Framebuffer& framebuffer) {
    m_backBufferDebugName = backBufferName;
    m_backBuffer          = framebuffer;
}

void RenderGraph::SetupSwapChain(const Swapchain& swapchain) {
    m_presentRenderPass = swapchain.GetRenderPass();
}

RenderGraphPass& RenderGraph::AddPass(const std::string& passName, RenderCommandQueueType type) {
    if(m_renderGraphPasses.contains(passName)) {
        WIND_CORE_WARN("{passName} is already existed!");
    }
    else {
        m_renderGraphPasses[passName] = ref::Create<RenderGraphPass>(m_device, passName, type);
    }

    return *m_renderGraphPasses[passName];
}

void RenderGraph::Exec() {
    for(const auto& graphPass : m_renderGraphPasses) {
        
    }
}
} // namespace wind