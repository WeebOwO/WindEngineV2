#pragma once

#include "RenderBackend/Command.h"
#include "RenderBackend/SwapChain.h"
#include "Scene/Scene.h"

namespace wind {
class ReadBackBuffer;
class ComputeShader;

struct FrameParms {
    std::shared_ptr<CommandEncoder> m_encoder;
};

class SceneRenderer {
public:
    SceneRenderer();
    ~SceneRenderer();

    void Render(Swapchain& swapchain);
    u32  GetCurrentFrame();

private:
    void InitView();
    void ComputeTest();
    void BasePassRendering();

    GPUDevice& m_device;
    vk::Fence  m_tempComputeFence;

    FrameParms m_frameParams[Swapchain::MAX_FRAME_IN_FLIGHT];
    u32        m_frameNumber{0};
};
} // namespace wind