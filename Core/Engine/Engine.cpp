#include "Engine.h"

#include <thread>

#include "Base/Log.h"
#include "GLFW/glfw3.h"
#include "ECS/JobSystem.h"
#include "RenderBackend/Backend.h"

#include "Window.h"

namespace wind {

    void Func1() {
        WIND_CORE_INFO("Run func 1");
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    void Func2() {
        WIND_CORE_INFO("Run func 2");
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    void Func3() {
        WIND_CORE_INFO("Run func 3");
    }

    Engine::Engine(Window* window) {
        Init(window);
    }

    Engine::~Engine() {
        Quit();
    }
    
    void Engine::Run() {
        JobEntry(Func3, 3, {1, 2});
        JobEntry(Func1, 1);
        JobEntry(Func2, 2);

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