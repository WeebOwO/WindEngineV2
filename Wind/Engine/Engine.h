#pragma once

#include "std.h"

#include "Engine/RenderThread.h"

namespace wind
{
    class SceneRenderer;
    class Window;
    class Scene;

    using ImGuiCallBack = std::function<void(Engine&)>;

    class Engine
    {
    public:
        Engine(Scope<Window> window); // here use unique ptr to transfer ownership from editor to engine
        ~Engine();

        void Run();
        void SetImguiCallBack(ImGuiCallBack&& callback) { m_imguiCallback = std::move(callback); }

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
        uint32_t                              m_activeSceneIndex;
        std::chrono::steady_clock::time_point m_lastTickTimePoint {std::chrono::steady_clock::now()};
        RenderThread                          m_renderThread;
        Scope<SceneRenderer>                  m_sceneRenderer;
        ImGuiCallBack                         m_imguiCallback;
    };
} // namespace wind