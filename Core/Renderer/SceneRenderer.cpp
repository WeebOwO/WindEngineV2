#include "SceneRenderer.h"

#include "Base/Log.h"
#include "Scene/Scene.h"
#include "RenderBackend/Buffer.h"

namespace wind {
static bool test = true;
SceneRenderer::~SceneRenderer() {
    WIND_CORE_INFO("Destory {}", "Scene Renderer");
}

SceneRenderer::SceneRenderer() {
    testVec.resize(4);
    m_readBackBuffer = std::make_unique<ReadBackBuffer>(sizeof(u32) * testVec.size());
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