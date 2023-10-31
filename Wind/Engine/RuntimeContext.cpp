#include "RuntimeContext.h"

#include "ECS/JobSystem.h"
#include "RenderBackend/Device.h"

namespace wind {
RuntimeContext g_runtimeContext;

void ShaderMap::Init() {
    using namespace std::filesystem;
    std::stringstream stringstream; 
}

void RuntimeContext::Init() {
    // Core engine part
    Log::Init();
    JobSystem::Init();
    device = scope::Create<GPUDevice>();
}

void RuntimeContext::Quit() { 
    JobSystem::Quit(); 
}

std::filesystem::path GetPath(std::filesystem::path path) {
    static std::filesystem::path rootPath(RuntimeContext::projectRelative);
    auto r = rootPath.append(path.c_str());
    return r;
}
} // namespace wind