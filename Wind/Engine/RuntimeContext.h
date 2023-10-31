#pragma once

#include "std.h"

#include "shaderc/shaderc.hpp"

namespace wind {
// manage runtime resource and system
class RasterShader;
class ComputeShader;
class GPUDevice;

struct ShaderMap {
    void                                                Init();
    std::unordered_map<std::string, Ref<RasterShader>>  rasterShaderCache;
    std::unordered_map<std::string, Ref<ComputeShader>> computeShaderCache;
    shaderc::Compiler                                   shaderCompiler;
};

struct RuntimeContext {
    static constexpr std::string projectRelative = "../../../../";

    void Init();
    void Quit();

    Scope<ShaderMap> shaderMap;
    Scope<GPUDevice> device;
};

std::filesystem::path GetPath(std::filesystem::path path);

extern RuntimeContext g_runtimeContext;
} // namespace wind