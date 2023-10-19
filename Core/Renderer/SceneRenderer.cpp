#include "SceneRenderer.h"

#include "Base/Log.h"
#include "Resource/Loader.h"

#include "RenderBackend/Buffer.h"
#include "RenderBackend/Command.h"
#include "RenderBackend/ComputeShader.h"

#include "RenderGraph/RenderPass.h"

namespace wind {
void FrameParms::Init() {
    m_encoders[computeIndex] = ref::Create<ComputeEncoder>();
    m_encoders[renderIndex]  = ref::Create<RenderEncoder>();
}

void FrameParms::ResetCommanEncoders() {
    for (const auto& encoder : m_encoders) {
        encoder->Reset();
    }
}

SceneRenderer::~SceneRenderer() { WIND_CORE_INFO("Destory {}", "Scene Renderer"); }

SceneRenderer::SceneRenderer() : m_device(Backend::GetGPUDevice()) {
    auto vkDevice = m_device.GetVkDeviceHandle();
    m_renderGraph = ref::Create<RenderGraph>(m_device);
    // create frame parms
    for (auto& frameParms : m_frameParams) {
        frameParms.Init();
    }
}

u32 SceneRenderer::GetCurrentFrame() { return m_frameNumber; }

void SceneRenderer::ResetFrameParams() { m_frameParams[GetCurrentFrame()].ResetCommanEncoders(); }

void SceneRenderer::Render(Swapchain& swapchain) {
    swapchain.SetFrameNumber(m_frameNumber);
    u32 imageIndex = swapchain.AcquireNextImage().value();
    ResetFrameParams();

    // init render graph
    m_renderGraph->SetupSwapChain(swapchain, imageIndex);
    m_renderGraph->SetGraphicsEncoder(
        m_frameParams[GetCurrentFrame()].GetEncoder(FrameParms::renderIndex));
    m_renderGraph->SetComputeEncoder(
        m_frameParams[GetCurrentFrame()].GetEncoder(FrameParms::computeIndex));
    m_renderGraph->ImportBackBuffer("BackBuffer");

    PresentPass();

    m_renderGraph->Exec();
    m_frameNumber = (m_frameNumber + 1) % Swapchain::MAX_FRAME_IN_FLIGHT;
}

void SceneRenderer::PresentPass() {
    auto& presentPass = m_renderGraph->AddPass("PresentPass", RenderCommandQueueType::Graphics);
    presentPass.MarkWriteBackBuffer();
    presentPass.SetRenderExecCallBack([](RenderEncoder& encoder) {
        // empty now
    });
}
} // namespace wind