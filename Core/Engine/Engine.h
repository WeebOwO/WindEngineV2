#pragma once

#include "std.h"

namespace wind {
class SceneRenderer;
class Window;

class Engine {
public:
    Engine(Scope<Window> window); // here use unique ptr to transfer ownership from editor to engine
    ~Engine();

    void Run();
    
private:
    void Init();
    void PostInit();
    void Quit();
    void RenderTick(float delta);
    void LogicTick(float delta);

    float CalcDeltaTime();

    Scope<Window> m_window;
    Scope<SceneRenderer> m_sceneRenderer;
    
    std::chrono::steady_clock::time_point m_lastTickTimePoint{std::chrono::steady_clock::now()};
};

} // namespace wind