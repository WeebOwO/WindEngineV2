#pragma once

#include "Renderer/SceneRenderer.h"

namespace wind {

class Window;
class Engine {
public:
    Engine(Window& window);
    ~Engine();

    void Run();
    
private:
    void Init();
    void Quit();
    void RenderTick(float delta);
    void LogicTick(float delta);

    float CalcDeltaTime();

    Window& m_window;
    
    std::unique_ptr<SceneRenderer> m_sceneRenderer;
    std::chrono::steady_clock::time_point m_lastTickTimePoint{std::chrono::steady_clock::now()};
};

} // namespace wind