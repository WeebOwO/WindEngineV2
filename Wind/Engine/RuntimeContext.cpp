#include "RuntimeContext.h"

#include <shaderc/shaderc.hpp>

#include "ECS/JobSystem.h"
#include "RenderBackend/Device.h"

namespace wind {
RuntimeContext g_runtimeContext;

void RuntimeContext::Init() {
    // Core engine part
    Log::Init();
    JobSystem::Init();
    device = scope::Create<GPUDevice>();
    // init project path
    auto currentPath = std::filesystem::current_path();

    pathManager.projectPath = currentPath.parent_path().parent_path().parent_path().parent_path();
    pathManager.shaderPath = pathManager.projectPath.append("Shaders");
    std::cout << pathManager.shaderPath << std::endl;
    
    auto InitShaderMap = [&]() {
        shaderc::Compiler compiler;
        
    };
}

void RuntimeContext::Quit() { 
    JobSystem::Quit(); 
}

std::filesystem::path GetPath(std::filesystem::path path) {
    return std::filesystem::path();
}
} // namespace wind