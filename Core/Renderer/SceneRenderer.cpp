#include "SceneRenderer.h"

#include "Base/Log.h"
#include "RenderBackend/Buffer.h"
#include "RenderBackend/ComputeShader.h"
#include "Renderer/SceneRenderer.h"
#include "Resource/Loader.h"
#include "Scene/Scene.h"
#include <memory>
#include <vcruntime_string.h>

namespace wind {
static bool test = true;

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
    if(test) {
        ComputeTest();
        test = false;
    }
    ++m_frameNumber;
}

void SceneRenderer::ComputeTest() {
    auto vkDevice = m_device.GetVkDeviceHandle();

    static std::unique_ptr<ComputeShader> computeShader =
        std::make_unique<ComputeShader>(io::LoadBinary<u32>("ComputeTest.comp.spv"));

    auto cmdBuffer = m_frameParams[GetCurrentFrame()].m_encoder->Begin();
    std::vector<i32> data = {1, 2, 3, 4};

    static ReadBackBuffer buffer(sizeof(i32) * data.size(), vk::BufferUsageFlagBits::eStorageBuffer);

    vk::DescriptorBufferInfo m_bufferInfo{
        .buffer = buffer.GetNativeHandle(), .offset = 0, .range = buffer.GetByteSize()};

    computeShader->BindResource("Buffer", m_bufferInfo);
    
    computeShader->Bind(cmdBuffer);
    cmdBuffer.dispatch(4, 1, 1);

    cmdBuffer.end();

    vk::SubmitInfo submitInfo{.commandBufferCount = 1, .pCommandBuffers = &cmdBuffer};

    m_device.GetComputeQueue().submit(submitInfo);

    vkDevice.waitIdle();
    cmdBuffer.reset();

    memcpy(data.data(), buffer.MapMemory(), buffer.GetByteSize());

    for(auto val : data) {
        WIND_CORE_INFO("Test is {}", val);
    }
}
} // namespace wind