#pragma once

#include "std.h"

namespace wind {
class RasterShader;
class ComputeShader;

class GlobalShaderMap {
public:
    void Init();

    Ref<RasterShader>  GetGraphcisShader(const std::string& shaderName);
    Ref<ComputeShader> GetComputeShader(const std::string& shaderName);

private:
    std::unordered_map<std::string, Ref<RasterShader>>  m_rasterShaderCache;
    std::unordered_map<std::string, Ref<ComputeShader>> m_computeShaderCache;
};
}; // namespace wind