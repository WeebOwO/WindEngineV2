#include "RenderGraph.h"

#include "ResourceRegistry.h"
#include "ResourceNode.h"

#include "Core/Log.h"

#include "Engine/RenderThread.h"
#include "Engine/RuntimeContext.h"

#include "RenderBackend/SwapChain.h"

namespace wind {

RenderGraph::RenderGraph() {}

void RenderGraph::SetupSwapChain(const Swapchain& swapchain) { m_swapchain = &swapchain; }
void RenderGraph::SetupFrameData(FrameParms& frameData) { m_currentFrameData = &frameData; }

void RenderGraph::Exec() {
    auto renderEncoder = m_currentFrameData->renderEncoder; 
    
    for(const auto& [name, node] : m_passNodes) {
        ResourceRegistry registry(*this, node.get());
        node->Execute(registry, *renderEncoder);
    }
    
}

RenderGraph::Builder RenderGraph::AddPassInternal(const std::string&         name,
                                                  Scope<RenderGraphPassBase> pass) {
    Scope<PassNode> node = scope::Create<RenderPassNode>(*this, name, std::move(pass));
    auto rawPtr = node.get();
    m_passNodes[name] = std::move(node);
    return Builder{*this, rawPtr};
}

void RenderGraph::Compile() { m_dirty = false; }

vk::RenderingInfo RenderGraph::GetPresentRenderingInfo() const noexcept {
    auto index = m_currentFrameData->swapchainImageIndex;
    return m_swapchain->GetRenderingInfo(index);
}
} // namespace wind