#pragma once

#include "Scene/Scene.h"
#include "RenderBackend/SwapChain.h"

namespace wind {
class ReadBackBuffer;
class ComputeShader;

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
    std::vector<u32> testVec;

    std::unique_ptr<ComputeShader> m_computeTestShader;
};
} // namespace wind