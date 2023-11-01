#pragma once

#include "std.h"

#include "RenderBackend/VulkanHeader.h"
#include <unordered_map>

namespace wind {
// manage runtime resource and system
class RasterShader;
class ComputeShader;
class GPUDevice;

struct ShaderMap {
    std::unordered_map<std::string, Ref<RasterShader>>  rasterShaderCache;
    std::unordered_map<std::string, Ref<ComputeShader>> computeShaderCache;
    std::unordered_map<std::string, vk::ShaderModule>   shaderModuleCache;
};

struct PathManager {
    std::filesystem::path projectPath;
    std::filesystem::path shaderPath;
};

struct PsoManager {
    std::unordered_map<const std::string, vk::Pipeline> psocache;
};

struct RuntimeContext {
    void Init();
    void Quit();

    Scope<ShaderMap>  shaderMap;
    Scope<GPUDevice>  device;
    Scope<PsoManager> psoManager;

    PathManager pathManager;
};

std::filesystem::path GetPath(std::filesystem::path path);

extern RuntimeContext g_runtimeContext;
} // namespace wind