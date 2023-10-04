#include "SceneRenderer.h"

#include "Base/Log.h"
#include "RenderBackend/Buffer.h"
#include "RenderBackend/Command.h"
#include "RenderBackend/ComputeShader.h"
#include "Renderer/SceneRenderer.h"
#include "Resource/Loader.h"
#include "Scene/Scene.h"

namespace wind {

SceneRenderer::~SceneRenderer() { WIND_CORE_INFO("Destory {}", "Scene Renderer"); }

SceneRenderer::SceneRenderer() : m_device(Backend::GetGPUDevice()) {
    auto vkDevice = m_device.GetVkDeviceHandle();

    // create frame parms
    for (auto& frameParms : m_frameParams) {
        frameParms.m_encoder = std::make_shared<CommandEncoder>(RenderCommandQueueType::All);
    }
}

u32 SceneRenderer::GetCurrentFrame() { return m_frameNumber % Swapchain::MAX_FRAME_IN_FLIGHT; }

void SceneRenderer::Render(Swapchain& swapchain) {
    if (!m_frameNumber) {
        ComputeTest();
    }
    ++m_frameNumber;
}

void SceneRenderer::ComputeTest() {
    auto                                  vkDevice = m_device.GetVkDeviceHandle();
    std::vector<i32>                      data     = {1, 2, 3, 4};

    std::unique_ptr<ComputeShader> computeShader =
        std::make_unique<ComputeShader>(io::LoadBinary<u32>("ComputeTest.comp.spv"));
    ReadBackBuffer buffer(sizeof(i32) * data.size(),
                                 vk::BufferUsageFlagBits::eStorageBuffer);

    vk::DescriptorBufferInfo m_bufferInfo{
        .buffer = buffer.GetNativeHandle(), .offset = 0, .range = buffer.GetByteSize()};
    computeShader->BindResource("Buffer", m_bufferInfo);

    ImmCommand command;
    command.PushTask([&](const vk::CommandBuffer& cmdBuffer) {
        computeShader->Bind(cmdBuffer);
        cmdBuffer.dispatch(4, 1, 1);
    });

    command.Submit();

    vkDevice.waitIdle();
    memcpy(data.data(), buffer.MapMemory(), buffer.GetByteSize());

    for (auto val : data) {
        WIND_CORE_INFO("Test is {}", val);
    }
}
} // namespace wind