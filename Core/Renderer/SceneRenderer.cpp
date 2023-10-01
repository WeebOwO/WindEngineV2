#include "SceneRenderer.h"

#include "Base/Log.h"
#include "Resource/Loader.h"
#include "Scene/Scene.h"
#include "RenderBackend/Buffer.h"
#include "RenderBackend/ComputeShader.h"

namespace wind {
static bool test = true;
SceneRenderer::~SceneRenderer() {
    WIND_CORE_INFO("Destory {}", "Scene Renderer");
}

SceneRenderer::SceneRenderer() {
    testVec.resize(4);
    m_readBackBuffer = std::make_unique<ReadBackBuffer>(sizeof(u32) * testVec.size());

    auto binaryCode = io::LoadBinary<u32>("ComputeTest.comp.spv");
    m_computeTestShader = std::make_unique<ComputeShader>(binaryCode);
}

void SceneRenderer::Render(Swapchain& swapchain) {
    if (test) {
        ComputeTest();
        test = false;
    }
}

void SceneRenderer::ComputeTest() { 
    WIND_CORE_INFO("ComputeTestBegin"); 
    
}
} // namespace wind