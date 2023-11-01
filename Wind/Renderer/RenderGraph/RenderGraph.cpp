#include "RenderGraph.h"

#include "Core/Log.h"

#include "RenderPass.h"

#include "RenderBackend/Command.h"
#include "RenderBackend/SwapChain.h"

#include "Renderer/SceneRenderer.h"

namespace wind {

RenderGraph::RenderGraph(GPUDevice& device) : m_device(device) {}

void RenderGraph::ImportBackBuffer(const std::string& backBufferName) {
    m_backBufferDebugName = backBufferName;
}

void RenderGraph::SetupSwapChain(const Swapchain& swapchain) { m_swapchain = &swapchain; }

void RenderGraph::SetupFrameData(FrameParms& frameData) { m_currentFrameData = &frameData; }

RenderGraphPass& RenderGraph::AddPass(const std::string& passName, RenderCommandQueueType type) {
    if (!m_renderGraphPasses.contains(passName)) {
        m_dirty                       = true;
        m_renderGraphPasses[passName] = ref::Create<RenderGraphPass>(*this, passName, type);
    }

    return *m_renderGraphPasses[passName];
}

void RenderGraph::Exec() {
    if(m_dirty) {
        Compile();
    }
    
    auto vkDevice      = m_device.GetVkDeviceHandle();
    auto renderEncoder = m_currentFrameData->renderEncoder;

    renderEncoder->Begin();

    for (const auto& [passDebugName, graphPass] : m_renderGraphPasses) {
        if (graphPass->IsWriteToBackBuffer()) {
            vk::RenderPassBeginInfo beginInfo{
                .renderPass  = m_swapchain->GetRenderPass(),
                .framebuffer = m_swapchain->GetFrameBuffer(m_currentFrameData->swapchainImageIndex),
                .renderArea  = vk::Rect2D{.offset = {},
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
        m_swapchain->SubmitCommandBuffer(renderEncoder->Finish(), m_currentFrameData->flightFence,
                                         m_currentFrameData->imageAvailableSemaphore,
                                         m_currentFrameData->renderFinishedSemaphore,
                                         m_currentFrameData->swapchainImageIndex);
    }
    m_dirty = false;
}

void RenderGraph::Compile() {
    for (auto& [debugName, pass] : m_renderGraphPasses) {
        if (pass->m_rastershader != nullptr && pass->m_computeShader != nullptr) {
            // todo : async compute start
        } else if (pass->m_rastershader != nullptr) {
            // todo : graphics pass
        } else if (pass->m_computeShader != nullptr) {
            // todo : comptue pass
        }
    }
}
} // namespace wind