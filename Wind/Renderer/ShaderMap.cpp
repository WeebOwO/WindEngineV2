#include "ShaderMap.h"

#include "RenderBackend/ComputeShader.h"
#include "RenderBackend/RasterShader.h"

namespace wind {
void GlobalShaderMap::Init() {}

Ref<RasterShader> GlobalShaderMap::GetGraphcisShader(const std::string& shaderName)  {
    return m_rasterShaderCache[shaderName];
}

Ref<ComputeShader> GlobalShaderMap::GetComputeShader(const std::string& shaderName) {
    return m_computeShaderCache[shaderName];
}

} // namespace wind