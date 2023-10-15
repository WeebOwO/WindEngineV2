#pragma once

#include "std.h"

#include "RenderBackend/Command.h"
#include "Renderbackend/SwapChain.h"

namespace wind {
class ReadBackBuffer;
class ComputeShader;
class ComputeEncoder;
class RenderEncoder;
class Swapchain;

struct FrameParms {
    void Init();
    void ResetCommanEncoders();
    auto GetEncoder(u32 index) { return m_encoders[index].get(); }

    static constexpr u32                 computeIndex = 0, renderIndex = 1;
    std::array<Scope<CommandEncoder>, 2> m_encoders;
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
    void ResetFrameParams();

    void BasePassRendering();
    void PresentPass(Swapchain& swapchain, u32 imageIndex);

    GPUDevice& m_device;

    FrameParms m_frameParams[Swapchain::MAX_FRAME_IN_FLIGHT];
    u32        m_frameNumber{0};
};
} // namespace wind