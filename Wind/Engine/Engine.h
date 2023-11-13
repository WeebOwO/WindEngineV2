#pragma once

#include "std.h"

#include "Engine/LayerStack.h"
#include "Engine/RenderThread.h"

namespace wind {
class SceneRenderer;
class Window;
class Scene;

class Engine {
public:
    Engine(Scope<Window> window); // here use unique ptr to transfer ownership from editor to engine
    ~Engine();

    void Run();

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* layer);
    void PopLayer(Layer* layer);
    void PopOverlay(Layer* layer);

private:
    void Init();
    void PostInit();
    void Quit();

    void RenderTick(float delta);
    void LogicTick(float delta);

    void  LoadScene();
    float CalcDeltaTime();

    Scope<Window>                         m_window;
    std::vector<Scope<Scene>>             m_scenes;
    u32                                   m_activeSceneIndex;
    std::chrono::steady_clock::time_point m_lastTickTimePoint{std::chrono::steady_clock::now()};
    LayerStack                            m_layerStack; // store app's function
    RenderThread                          m_renderThread;
};
} // namespace wind