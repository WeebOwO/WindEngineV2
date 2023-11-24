#include "RenderGraph.h"

#include "Core/Log.h"
#include "Engine/RuntimeContext.h"
#include "RenderBackend/Command.h"
#include "RenderBackend/SwapChain.h"
#include "Renderer/SceneRenderer.h"

namespace wind {

RenderGraph::RenderGraph() {}

void RenderGraph::SetupSwapChain(const Swapchain& swapchain) { m_swapchain = &swapchain; }

void RenderGraph::SetupFrameData(FrameParms& frameData) { m_currentFrameData = &frameData; }

void RenderGraph::Exec() {
    if (m_dirty) { Compile(); }

    auto vkDevice      = RuntimeUtils::GetVulkanDevice();
    auto renderEncoder = m_currentFrameData->renderEncoder;

    auto swapchainImageIndex = m_currentFrameData->swapchainImageIndex;
    renderEncoder->Begin();

    // start barrier ot
    renderEncoder->TransferImageLayout(
        m_swapchain->GetImage(swapchainImageIndex), vk::AccessFlagBits::eNone,
        vk::AccessFlagBits::eColorAttachmentWrite, vk::ImageLayout::eUndefined,
        vk::ImageLayout::eColorAttachmentOptimal, vk::PipelineStageFlagBits::eTopOfPipe,
        vk::PipelineStageFlagBits::eColorAttachmentOutput,
        vk::ImageSubresourceRange{vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1});

    if (m_swapchain) {
        // end sync with backbuffer
        renderEncoder->TransferImageLayout(
            m_swapchain->GetImage(swapchainImageIndex), vk::AccessFlagBits::eColorAttachmentWrite,
            vk::AccessFlagBits::eNone, vk::ImageLayout::eColorAttachmentOptimal,
            vk::ImageLayout::ePresentSrcKHR, vk::PipelineStageFlagBits::eColorAttachmentOutput,
            vk::PipelineStageFlagBits::eBottomOfPipe,
            vk::ImageSubresourceRange{vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1});

        m_swapchain->SubmitCommandBuffer(renderEncoder->Finish(), m_currentFrameData->flightFence,
                                         m_currentFrameData->imageAvailableSemaphore,
                                         m_currentFrameData->renderFinishedSemaphore,
                                         swapchainImageIndex);
    }
}

RenderGraph::Builder RenderGraph::AddPassInternal(const std::string&         name,
                                                  Scope<RenderGraphPassBase> pass) {
    m_renderPasses[name] = std::move(pass);
    return Builder{this};
}

void RenderGraph::Compile() { m_dirty = false; }
} // namespace wind