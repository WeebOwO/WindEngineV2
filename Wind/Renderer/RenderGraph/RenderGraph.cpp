#include "RenderGraph.h"

#include "ResourceNode.h"
#include "ResourceRegistry.h"

#include "Core/Log.h"

#include "Engine/RenderThread.h"
#include "Engine/RuntimeContext.h"

#include "Backend/Command.h"
#include "Backend/SwapChain.h"

namespace wind {

RenderGraph::RenderGraph() {}

void RenderGraph::SetupSwapChain(const Swapchain& swapchain) { m_swapchain = &swapchain; }
void RenderGraph::SetupFrameData(FrameParms& frameData) { m_currentFrameData = &frameData; }

void RenderGraph::Exec() {
    auto renderEncoder = m_currentFrameData->renderEncoder;
    renderEncoder->Begin();
    SwapchainStartTrans();

    for (const auto& [name, node] : m_passNodes) {
        ResourceRegistry registry(*this, node.get());
        node->Execute(registry, *renderEncoder);
    }
    
    if (m_swapchain) {
        SwapchainEndTrans();
        m_swapchain->SubmitCommandBuffer(renderEncoder->Finish(), m_currentFrameData->flightFence,
                                         m_currentFrameData->imageAvailableSemaphore,
                                         m_currentFrameData->renderFinishedSemaphore,
                                         m_currentFrameData->swapchainImageIndex);
    }
}

RenderGraph::Builder RenderGraph::AddPassInternal(const std::string&         name,
                                                  Scope<RenderGraphPassBase> pass) {
    Scope<PassNode> node   = scope::Create<RenderPassNode>(*this, name, std::move(pass));
    auto            rawPtr = node.get();
    m_passNodes[name]      = std::move(node);
    return Builder{*this, rawPtr};
}

void RenderGraph::Compile() { m_dirty = false; }

vk::RenderingInfo RenderGraph::GetPresentRenderingInfo() const noexcept {
    auto index = m_currentFrameData->swapchainImageIndex;
    return m_swapchain->GetRenderingInfo(index);
}

void RenderGraph::SwapchainStartTrans() {
    m_currentFrameData->renderEncoder->TransferImageLayout(
        m_swapchain->GetImage(m_currentFrameData->swapchainImageIndex), vk::AccessFlagBits::eNone,
        vk::AccessFlagBits::eColorAttachmentWrite, vk::ImageLayout::eUndefined,
        vk::ImageLayout::eColorAttachmentOptimal, vk::PipelineStageFlagBits::eTopOfPipe,
        vk::PipelineStageFlagBits::eColorAttachmentOutput,
        vk::ImageSubresourceRange{.aspectMask     = vk::ImageAspectFlagBits::eColor,
                                  .baseMipLevel   = 0,
                                  .levelCount     = 1,
                                  .baseArrayLayer = 0,
                                  .layerCount     = 1});
}

void RenderGraph::SwapchainEndTrans() {
    m_currentFrameData->renderEncoder->TransferImageLayout(
        m_swapchain->GetImage(m_currentFrameData->swapchainImageIndex),
        vk::AccessFlagBits::eColorAttachmentWrite, vk::AccessFlagBits::eNone,
        vk::ImageLayout::eColorAttachmentOptimal, vk::ImageLayout::ePresentSrcKHR,
        vk::PipelineStageFlagBits::eColorAttachmentOutput, vk::PipelineStageFlagBits::eBottomOfPipe,
        vk::ImageSubresourceRange{.aspectMask     = vk::ImageAspectFlagBits::eColor,
                                  .baseMipLevel   = 0,
                                  .levelCount     = 1,
                                  .baseArrayLayer = 0,
                                  .layerCount     = 1});
}

} // namespace wind