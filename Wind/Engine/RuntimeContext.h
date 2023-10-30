#pragma once

#include "std.h"

namespace wind {
// manage runtime resource and system
class RasterShader;
class ComputeShader;
class GPUDevice;

struct ShaderMap {
    std::unordered_map<std::string, Ref<RasterShader>>  rasterShaderCache;
    std::unordered_map<std::string, Ref<ComputeShader>> computeShaderCache;
};

class RuntimeContext {
public:
    void Init();
    void Quit();

    Scope<ShaderMap> shaderMap;
    Scope<GPUDevice> device;
};

extern RuntimeContext g_runtimeContext;
} // namespace wind