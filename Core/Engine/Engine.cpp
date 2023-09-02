#include "Engine.h"

#include "Base/Log.h"

namespace wind {
    Engine::Engine() {
        Init();
    }

    Engine::~Engine() {
        Quit();
    }
    
    void Engine::Run() {
        
    }

    void Engine::Init() {
        Log::Init();
        WIND_CORE_INFO("Init the engine core!");
    }

    void Engine::Quit() {
        
    }
}