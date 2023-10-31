#include "RasterShader.h"

namespace wind {

RasterShader::RasterShader(const std::string& name, Ref<vk::ShaderModule> vertexModule,
                           Ref<vk::ShaderModule> fragShaderModule) noexcept
    : m_shaderModules({vertexModule, fragShaderModule}) {
    SetShaderName(name);
}

Ref<RasterShader> Compile(ShaderMap& shaderMap, const std::string& vertexFilePath,
                          const std::string& fragfilePath) {
    return nullptr;
}
} // namespace wind