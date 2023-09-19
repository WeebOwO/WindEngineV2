#pragma once

#include <memory>

#include "RenderBackend/SwapChain.h"
#include "Scene/Scene.h"

namespace wind {
class SceneRenderer {
public:
    SceneRenderer();
    auto Render(Swapchain& swapchain) -> void;

private:
    void                       InitView();
    void                       BasePassRendering();
};
} // namespace wind