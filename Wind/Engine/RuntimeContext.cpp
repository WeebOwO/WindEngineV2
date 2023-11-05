#include "RuntimeContext.h"

#include <shaderc/shaderc.hpp>

#include "ECS/JobSystem.h"
#include "RenderBackend/ComputeShader.h"
#include "RenderBackend/Device.h"
#include "RenderBackend/RasterShader.h"
#include "Renderer/Material.h"

namespace wind {
RuntimeContext g_runtimeContext;

void ShaderMap::CacheRasterShader(Ref<RasterShader> shader) {
    m_rasterShaderCache[shader->GetShaderName()] = shader;
}

void ShaderMap::CacheComputeShader(Ref<ComputeShader> shader) {
    m_computeShaderCache[shader->GetShaderName()] = shader;
}

void RuntimeContext::Init() {
    // Core engine part
    Log::Init();
    JobSystem::Init();
    device = scope::Create<GPUDevice>();
    // init project path
    auto currentPath = std::filesystem::current_path();

    pathManager.projectPath = currentPath.parent_path().parent_path().parent_path().parent_path();
    pathManager.shaderPath  = pathManager.projectPath.append("Shaders");
    // init shader map
    shaderMap = scope::Create<ShaderMap>();
    shaderMap->CacheRasterShader(
        RasterShader::Create("BasePassShader", "Triangle.vert.spv", "Triangle.frag.spv"));
    // init material manager
}

void RuntimeContext::Quit() { JobSystem::Quit(); }

std::filesystem::path GetPath(std::filesystem::path path) { return std::filesystem::path(); }
} // namespace wind