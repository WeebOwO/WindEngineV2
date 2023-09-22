#include "SceneRenderer.h"

#include "Base/Log.h"
#include "Scene/Scene.h"
#include "RenderBackend/Backend.h"

namespace wind {
static bool test = true;

SceneRenderer::SceneRenderer() {
     
}

void SceneRenderer::Render(Swapchain& swapchain) {
    if (test) {
        ComputeTest();
        test = false;
    }
}

void SceneRenderer::ComputeTest() { WIND_CORE_INFO("ComputeTestBegin"); }
} // namespace wind