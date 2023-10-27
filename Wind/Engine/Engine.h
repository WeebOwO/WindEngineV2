#pragma once

#include "std.h"

namespace wind {
class SceneRenderer;
class Window;
class Scene;

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

    void  LoadScene();
    float CalcDeltaTime();

    Scope<Window>                         m_window;
    Scope<SceneRenderer>                  m_sceneRenderer;
    std::vector<Scope<Scene>>             m_scenes;
    u32                                   m_activeSceneIndex;
    std::chrono::steady_clock::time_point m_lastTickTimePoint{std::chrono::steady_clock::now()};
};
} // namespace wind