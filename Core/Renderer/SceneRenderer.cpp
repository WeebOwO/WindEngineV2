#include "SceneRenderer.h"

#include "Scene/Scene.h"
#include "RenderBackend/Backend.h"

namespace wind {
    SceneRenderer::SceneRenderer(const Window& window) {
        m_swapchain = std::make_unique<Swapchain>(Backend::GetGPUDevice(), window);
    }
    
    void SceneRenderer::Render() {

    }
}