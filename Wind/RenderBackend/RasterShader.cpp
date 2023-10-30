#include "RasterShader.h"

namespace wind {

RasterShader::RasterShader(const std::string& name, Ref<vk::ShaderModule> vertexModule,
                           Ref<vk::ShaderModule> fragShaderModule)
    : m_shaderModules({vertexModule, fragShaderModule}) {
    SetShaderName(name);
}
} // namespace wind