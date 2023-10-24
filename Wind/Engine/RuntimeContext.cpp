#include "RuntimeContext.h"

#include "ECS/JobSystem.h"
#include "RenderBackend/Backend.h"

namespace wind {
RuntimeContext g_runtimeContext;

void RuntimeContext::Init() {
    Log::Init();
    Backend::Init();
    JobSystem::Init();
}

void RuntimeContext::Quit() { 
    JobSystem::Quit(); 
}
} // namespace wind