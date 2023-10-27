#include "SceneRenderer.h"

#include "View.h"

#include "Core/Log.h"
#include "Scene/Scene.h"

#include "Renderer/SceneRenderer.h"
#include "Resource/Loader.h"

#include "RenderBackend/Buffer.h"
#include "RenderBackend/Command.h"
#include "RenderBackend/ComputeShader.h"
#include "RenderBackend/Descriptor.h"

#include "RenderGraph/RenderPass.h"

namespace wind {
void FrameParms::Init(const vk::Device& device) {
    computeEncoder = ref::Create<ComputeEncoder>();
    renderEncoder  = ref::Create<RenderEncoder>();

    vk::FenceCreateInfo fenceCreateInfo{.flags = vk::FenceCreateFlagBits::eSignaled};
    flightFence = device.createFence(fenceCreateInfo);

    imageAvailableSemaphore = device.createSemaphore({});
    renderFinishedSemaphore = device.createSemaphore({});

    dynamicDescriptorAllocator = ref::Create<DescriptorAllocator>(device);
}

void FrameParms::Destroy(const vk::Device& device) {
    device.destroyFence(flightFence);
    device.destroySemaphore(imageAvailableSemaphore);
    device.destroySemaphore(renderFinishedSemaphore);
}

void FrameParms::ResetCommanEncoders() {
    computeEncoder->Reset();
    renderEncoder->Reset();
}

SceneRenderer::~SceneRenderer() {
    m_device.GetVkDeviceHandle().waitIdle();
    for (auto& frameData : m_frameParams) {
        frameData.Destroy(m_device.GetVkDeviceHandle());
    }
}

SceneRenderer::SceneRenderer() : m_device(Backend::GetGPUDevice()) {
    auto vkDevice = m_device.GetVkDeviceHandle();
    m_renderGraph = ref::Create<RenderGraph>(m_device);
    // create frame parms
    for (auto& frameParms : m_frameParams) {
        frameParms.Init(vkDevice);
    }
}

FrameParms& SceneRenderer::GetCurrentFrameData() { return m_frameParams[m_frameNumber]; }

void SceneRenderer::Render(Swapchain& swapchain, Scene& scene, View& view) {
    auto& frameData = GetCurrentFrameData();
    frameData.swapchainImageIndex =
        swapchain.AcquireNextImage(frameData.flightFence, frameData.imageAvailableSemaphore)
            .value();

    frameData.ResetCommanEncoders();

    // init render graph
    m_renderGraph->SetupSwapChain(swapchain);
    m_renderGraph->SetupFrameData(frameData);
    m_renderGraph->ImportBackBuffer("BackBuffer");

    // record render pass
    PresentPass();

    m_renderGraph->Exec();
    m_frameNumber = (m_frameNumber + 1) % Swapchain::MAX_FRAME_IN_FLIGHT;
}

void SceneRenderer::PresentPass() {
    auto& presentPass = m_renderGraph->AddPass("PresentPass", RenderCommandQueueType::Graphics);
    presentPass.MarkWriteBackBuffer();
    presentPass.SetRenderExecCallBack([](RenderEncoder& encoder) {

    });
}
} // namespace wind