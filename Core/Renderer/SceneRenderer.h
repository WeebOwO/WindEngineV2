#pragma once

#include <memory>

#include "Scene/Scene.h"
#include "RenderBackend/SwapChain.h"

namespace wind {
class SceneRenderer {
public: 
    SceneRenderer(const Window& window);
    void Render();
    
private:
    std::unique_ptr<Swapchain> m_swapchain;
};
} // namespace wind