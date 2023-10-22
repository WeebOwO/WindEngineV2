#include "Engine.h"

#include <tracy/Tracy.hpp>

#include "Core/Log.h"
#include "ECS/JobSystem.h"
#include "RenderBackend/Backend.h"
#include "Renderer/SceneRenderer.h"
#include "Renderer/View.h"
#include "Scene/Scene.h"
#include "Window.h"


namespace wind {
Engine::Engine(Scope<Window> window) : m_window(std::move(window)) {
    Init();
    PostInit();
}

Engine::~Engine() { Quit(); }

void Engine::LoadScene() {
    m_activeSceneIndex = 0;
    m_scenes.push_back(scope::Create<Scene>());
}

void Engine::Run() {
#ifdef TRACY_ENABLE
    WIND_CORE_INFO("Start Tracy");
#endif
    LoadScene();
    while (!glfwWindowShouldClose(m_window->GetWindow())) {
        ZoneScoped;
        float fs = CalcDeltaTime();
        LogicTick(fs);
        RenderTick(fs);
        FrameMark;
    }
}

void Engine::Init() {
    Log::Init();
    Backend::Init();
    JobSystem::Init();
    WIND_CORE_INFO("Init the engine core!");

    m_sceneRenderer = scope::Create<SceneRenderer>();
}

float Engine::CalcDeltaTime() {
    float dalta;
    {
        using namespace std::chrono;
        steady_clock::time_point tickTimePoint = steady_clock::now();
        auto timeSpan = duration_cast<duration<float>>(tickTimePoint - m_lastTickTimePoint);
        dalta         = timeSpan.count();

        m_lastTickTimePoint = tickTimePoint;
    }
    return dalta;
}

void Engine::PostInit() { m_window->Init(); }

void Engine::Quit() {
    JobSystem::Quit();
    WIND_CORE_INFO("Shutdown engine");
}

void Engine::RenderTick(float delta) {
    ZoneScopedN("RenderTick");
    View view{};
    m_sceneRenderer->Render(*m_window->GetSwapChain(), *m_scenes[m_activeSceneIndex], view);
}

void Engine::LogicTick(float delta) {
    ZoneScopedN("LogicTick");
    m_window->OnUpdate(delta);
}
} // namespace wind