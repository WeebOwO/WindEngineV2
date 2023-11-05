#pragma once

#include "std.h"

namespace vk {
class ShaderModule;
};

namespace wind {
// manage runtime resource and system
class RasterShader;
class ComputeShader;
class GPUDevice;
class MaterialManager;

class ShaderMap {
public:
    void CacheRasterShader(Ref<RasterShader> shader);
    void CacheComputeShader(Ref<ComputeShader> shader);

    Ref<RasterShader> GetRasterShader(const std::string& shaderName) {
        return m_rasterShaderCache[shaderName];
    }

    Ref<ComputeShader> GetComputeShader(const std::string& shaderName) {
        return m_computeShaderCache[shaderName];
    }

private:
    std::unordered_map<std::string, Ref<RasterShader>>  m_rasterShaderCache;
    std::unordered_map<std::string, Ref<ComputeShader>> m_computeShaderCache;
};

struct PathManager {
    std::filesystem::path projectPath;
    std::filesystem::path shaderPath;
};

struct RuntimeContext {
    void Init();
    void Quit();

    Scope<ShaderMap>       shaderMap;
    Scope<GPUDevice>       device;
    Scope<MaterialManager> materialManager;

    PathManager pathManager;
};

std::filesystem::path GetPath(std::filesystem::path path);

extern RuntimeContext g_runtimeContext;
} // namespace wind