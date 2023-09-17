#include "Engine.h"

#include "Base/Log.h"
#include "ECS/JobSystem.h"
#include "RenderBackend/Backend.h"

#include "Renderer/SceneRenderer.h"
#include "Window.h"

namespace wind {
    Engine::Engine(Window* window) {
        Init(window);
    }

    Engine::~Engine() {
        Quit();
    }
    
    void Engine::Run() {
        while(!glfwWindowShouldClose(m_window->GetWindow())) {
            LogicTick();
            RenderTick();
        }
    }

    void Engine::Init(Window* window) {
        Log::Init();
        WIND_CORE_INFO("Init the engine core!");
        Backend::Init();
        JobSystem::Init();
        m_window = window;
        m_sceneRenderer = std::make_unique<SceneRenderer>(window);
    }

    void Engine::Quit() {
        JobSystem::Quit();
        WIND_CORE_INFO("Shutdown engine");
    }

    void Engine::RenderTick() {
        
    }   

    void Engine::LogicTick() {
        glfwPollEvents();
    }
}