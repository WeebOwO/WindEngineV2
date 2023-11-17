#include "RenderThread.h"

// engine header
#include "Engine/RuntimeContext.h"
// renderer part
#include "RenderBackend/Command.h"
#include "Renderer/RenderGraph/RenderGraph.h"
#include "Renderer/RenderGraph/RenderPass.h"
#include "Renderer/SceneRenderer.h"
#include "Renderer/View.h"
// imgui
#include "imgui.h"

namespace wind {

void FrameParms::Init() {
    auto device    = RuntimeUtils::GetVulkanDevice();
    computeEncoder = ref::Create<ComputeEncoder>();
    renderEncoder  = ref::Create<RenderEncoder>();

    vk::FenceCreateInfo fenceCreateInfo{.flags = vk::FenceCreateFlagBits::eSignaled};
    flightFence = device.createFence(fenceCreateInfo);

    imageAvailableSemaphore = device.createSemaphore({});
    renderFinishedSemaphore = device.createSemaphore({});

    dynamicDescriptorAllocator = ref::Create<DescriptorAllocator>(device);
}

void FrameParms::Destroy() {
    auto device = RuntimeUtils::GetVulkanDevice();
    device.destroyFence(flightFence);
    device.destroySemaphore(imageAvailableSemaphore);
    device.destroySemaphore(renderFinishedSemaphore);
}

void FrameParms::ResetCommanEncoders() {
    computeEncoder->Reset();
    renderEncoder->Reset();
}

void RenderThread::Init() {
    m_sceneRenderer = scope::Create<SceneRenderer>();
    m_sceneRenderer->Init();

    for (auto& data : m_frameParams) {
        data.Init();
    }

    m_renderGraph = scope::Create<RenderGraph>();
}

void RenderThread::Quit() {
    auto device = RuntimeUtils::GetVulkanDevice();
    device.waitIdle();
    for (auto& data : m_frameParams) {
        data.Destroy();
    }
}

void RenderThread::RenderJob(const Swapchain& swapchain) {
    auto& frameData = GetCurrentFrameData();

    frameData.swapchainImageIndex =
        swapchain.AcquireNextImage(frameData.flightFence, frameData.imageAvailableSemaphore)
            .value();
    frameData.ResetCommanEncoders();

    m_renderGraph->SetupFrameData(frameData);
    m_renderGraph->SetupSwapChain(swapchain);

    // set viewport
    View           view; 
    ImGuiViewport* viewport = ImGui::GetMainViewport();

    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);

    auto viewportOffset = ImGui::GetCursorPos(); // includes tab bar
    auto viewportSize   = ImGui::GetContentRegionAvail();

    m_sceneRenderer->SetViewPort(viewportOffset.x, viewportOffset.y, viewportSize.x, viewportSize.y);
    m_sceneRenderer->Render(view, *m_renderGraph);
}

void RenderThread::NextFrame() {
    m_renderGraph->Exec();
    m_frameNumber = (m_frameNumber + 1) % MAX_FRAME_IN_FLIGHT;
}
} // namespace wind