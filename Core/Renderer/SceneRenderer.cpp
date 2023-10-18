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
    m_renderGraph->SetupSwapChain(swapchain);
    m_renderGraph->SetGraphicsEncoder(
        m_frameParams[GetCurrentFrame()].GetEncoder(FrameParms::renderIndex));
    m_renderGraph->SetComputeEncoder(
        m_frameParams[GetCurrentFrame()].GetEncoder(FrameParms::computeIndex));
    m_renderGraph->ImportBackBuffer("BackBuffer", swapchain.GetFrameBuffer(imageIndex));

    PresentPass();
    
    m_renderGraph->SubmitToSwapChain(swapchain);

    m_frameNumber = (m_frameNumber + 1) % Swapchain::MAX_FRAME_IN_FLIGHT;
}

void SceneRenderer::ComputeTest() {
    auto             vkDevice = m_device.GetVkDeviceHandle();
    std::vector<i32> data     = {1, 2, 3, 4};

    Scope<ComputeShader> computeShader =
        scope::Create<ComputeShader>("ComputeTest", io::LoadBinary<u32>("ComputeTest.comp.spv"));
    ReadBackBuffer buffer(sizeof(i32) * data.size(), vk::BufferUsageFlagBits::eStorageBuffer);

    vk::DescriptorBufferInfo m_bufferInfo{
        .buffer = buffer.GetNativeHandle(), .offset = 0, .range = buffer.GetByteSize()};

    computeShader->BindResource("Buffer", m_bufferInfo);

    auto computeContext = m_frameParams[GetCurrentFrame()]
                              .GetEncoder(FrameParms::computeIndex)
                              ->CreateComputeEncoder();

    computeContext->Begin();
    computeContext->BindComputShader(*computeShader);
    computeContext->Dispatch(2, 1, 1);

    auto           cmdBuffer = computeContext->Finish();
    vk::SubmitInfo submitInfo{.commandBufferCount = 1, .pCommandBuffers = &cmdBuffer};
    m_device.GetComputeQueue().submit(submitInfo);

    vkDevice.waitIdle();
    memcpy(data.data(), buffer.MapMemory(), buffer.GetByteSize());

    for (auto val : data) {
        WIND_CORE_INFO("Test is {}", val);
    }
}

void SceneRenderer::PresentPass() {

    // vk::RenderPass renderPass = swapchain.GetRenderPass();

    // vk::ClearColorValue color{std::array<float, 4>{0.3f, 0.3f, 0.3f, 0.3f}};

    // vk::ClearValue clearValue{.color = color};

    // vk::RenderPassBeginInfo passBeginInfo{
    //     .renderPass      = renderPass,
    //     .framebuffer     = swapchain.GetFrameBuffer(imageIndex),
    //     .renderArea      = vk::Rect2D{{0, 0}, {swapchain.GetWidth(), swapchain.GetHeight()}},
    //     .clearValueCount = 1,
    //     .pClearValues    = &clearValue};

    // auto renderContext =
    //     m_frameParams[GetCurrentFrame()].GetEncoder(FrameParms::renderIndex)->CreateRenderEncoder();

    // renderContext->Begin();

    // renderContext->BeginRenderPass(passBeginInfo);

    // renderContext->EndRenderPass();

    // swapchain.SubmitCommandBuffer(renderContext->Finish(), imageIndex);


    auto& presentPass = m_renderGraph->AddPass("PresentPass", RenderCommandQueueType::Graphics);
    presentPass.SetExecCallBack([](CommandEncoder& encoder) {
        // empty now
    });
}
} // namespace wind