#include "RuntimeContext.h"

#include "ECS/JobSystem.h"
#include "RenderBackend/Device.h"

namespace wind {
RuntimeContext g_runtimeContext;

void RuntimeContext::Init() {
    // Core engine part
    Log::Init();
    JobSystem::Init();
    device = scope::Create<GPUDevice>();
}

void RuntimeContext::Quit() { 
    JobSystem::Quit(); 
}
} // namespace wind