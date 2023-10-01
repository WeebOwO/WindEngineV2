#pragma once

#include "RenderBackend/SwapChain.h"
#include "Scene/Scene.h"

namespace wind {
class ReadBackBuffer;
class ComputeShader;

struct FrameParms {
    vk::CommandBuffer cmdBuffer;
};

class SceneRenderer {
public:
    SceneRenderer();
    ~SceneRenderer();

    auto Render(Swapchain& swapchain) -> void;

private:
    void InitView();
    void ComputeTest();
    void BasePassRendering();
    void UploadPass();

    std::unique_ptr<ReadBackBuffer> m_readBackBuffer;
    std::vector<u32>                testVec;

    std::unique_ptr<ComputeShader> m_computeTestShader;

    u32 m_frameNumber{0};
    vk::Fence m_tempComputeFence;
};
} // namespace wind