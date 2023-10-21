#pragma once

#include "std.h"

#include "RenderBackend/Command.h"
#include "Renderbackend/SwapChain.h"
#include "RenderBackend/Fwd.h"

#include "RenderGraph/RenderGraph.h"

namespace wind {
struct FrameParms {
    void Init();
    void ResetCommanEncoders();
    auto GetEncoder(u32 index) { return m_encoders[index]; }

    static constexpr u32               computeIndex = 0, renderIndex = 1;
    std::array<Ref<CommandEncoder>, 2> m_encoders;
};

class SceneRenderer {
public:
    SceneRenderer();
    ~SceneRenderer();

    void Render(Swapchain& swapchain);
    u32  GetCurrentFrame();

private:
    void InitView();
    void ResetFrameParams();
    
    void BasePassRendering();
    void PresentPass();

    GPUDevice& m_device;
    FrameParms m_frameParams[Swapchain::MAX_FRAME_IN_FLIGHT];
    u32        m_frameNumber{0};
    Ref<RenderGraph> m_renderGraph;
};
} // namespace wind