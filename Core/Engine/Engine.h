#pragma once

#include "Renderer/SceneRenderer.h"

namespace wind {

class Window;
class Engine {
public:
    Engine(Window* window);
    ~Engine();

    void Run();
    
private:
    void Init(Window* window);
    void Quit();
    void RenderTick();
    void LogicTick();

    Window* m_window;
    std::unique_ptr<SceneRenderer> m_sceneRenderer;
};

} // namespace wind