#include "RenderGraph.h"

#include "Base/Log.h"
#include "RenderBackend/Command.h"
#include "RenderBackend/SwapChain.h"

#include "RenderPass.h"

namespace wind {

RenderGraph::RenderGraph(GPUDevice& device) : m_device(device) {}

void RenderGraph::ImportBackBuffer(const std::string& backBufferName) {
    m_backBufferDebugName = backBufferName;
}

void RenderGraph::SetupSwapChain(const Swapchain& swapchain, u32 imageIndex) {
    m_swapchain           = &swapchain;
    m_swapchainImageIndex = imageIndex;
}

RenderGraphPass& RenderGraph::AddPass(const std::string& passName, RenderCommandQueueType type) {
    if (m_renderGraphPasses.contains(passName)) {
        //
    } else {
        m_renderGraphPasses[passName] = ref::Create<RenderGraphPass>(*this, passName, type);
    }

    return *m_renderGraphPasses[passName];
}

void RenderGraph::Exec() {
    auto vkDevice      = m_device.GetVkDeviceHandle();
    auto renderEncoder = m_graphicsEncoder->CreateRenderEncoder();
    renderEncoder->Begin();
    
    for (const auto& [passDebugName, graphPass] : m_renderGraphPasses) {
        if (graphPass->IsWriteToBackBuffer()) {
            vk::RenderPassBeginInfo beginInfo{
                .renderPass      = m_swapchain->GetRenderPass(),
                .framebuffer     = m_swapchain->GetFrameBuffer(m_swapchainImageIndex),
                .renderArea      = vk::Rect2D{.offset = {},
                                              .extent = {.width  = m_swapchain->GetWidth(),
                                                         .height = m_swapchain->GetHeight()}},
                .clearValueCount = 1,
                .pClearValues    = m_swapchain->GetClearValue()};
            renderEncoder->BeginRenderPass(beginInfo);
            graphPass->m_renderExecCallback(*renderEncoder);
            renderEncoder->EndRenderPass();
        }
    }

    if (m_swapchain) {
        m_swapchain->SubmitCommandBuffer(renderEncoder->Finish(), m_swapchainImageIndex);
    }
}
} // namespace wind