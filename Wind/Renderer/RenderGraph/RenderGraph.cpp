#include "RenderGraph.h"

#include "Core/Log.h"
#include "RenderBackend/Command.h"
#include "RenderBackend/SwapChain.h"
#include "RenderPass.h"
#include "Renderer/SceneRenderer.h"

namespace wind {

RenderGraph::RenderGraph(GPUDevice& device) : m_device(device) {}

void RenderGraph::ImportBackBuffer(const std::string& backBufferName) {
    m_backBufferDebugName = backBufferName;
}

void RenderGraph::SetupSwapChain(const Swapchain& swapchain) { m_swapchain = &swapchain; }

void RenderGraph::SetupFrameData(FrameParms& frameData) { m_currentFrameData = &frameData; }

RenderGraphPass* RenderGraph::AddPass(const std::string& passName, RenderCommandQueueType type) {
    if (!m_renderGraphPasses.contains(passName)) {
        m_dirty                       = true;
        m_renderGraphPasses[passName] = ref::Create<RenderGraphPass>(*this, passName, type);
    }

    return m_renderGraphPasses[passName].get();
}

void RenderGraph::Exec() {
    if (m_dirty) { Compile(); }

    auto vkDevice      = m_device.GetVkDeviceHandle();
    auto renderEncoder = m_currentFrameData->renderEncoder;

    auto swapchainImageIndex = m_currentFrameData->swapchainImageIndex;
    renderEncoder->Begin();

    vk::Rect2D rect{.offset = {}, .extent{m_swapchain->GetWidth(), m_swapchain->GetHeight()}};
    for (const auto& [passDebugName, graphPass] : m_renderGraphPasses) {
        if (graphPass->IsWriteToBackBuffer()) {
            auto renderingInfo = m_swapchain->GetRenderingInfo(swapchainImageIndex);
            renderEncoder->BeginRendering(renderingInfo);
            graphPass->m_renderExecCallback(*renderEncoder);
            renderEncoder->EndRendering();
        }
    }

    if (m_swapchain) {
        m_swapchain->SubmitCommandBuffer(renderEncoder->Finish(), m_currentFrameData->flightFence,
                                         m_currentFrameData->imageAvailableSemaphore,
                                         m_currentFrameData->renderFinishedSemaphore,
                                         swapchainImageIndex);
    }
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
    m_dirty = false;
}
} // namespace wind