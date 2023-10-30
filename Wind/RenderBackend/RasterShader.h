#pragma once

#include "Shader.h"

namespace wind {
class Material;
class VertexFactory;

class RasterShader : public Shader {
public:
    friend class GraphicsPipelineBuilder;

    RasterShader(const std::string& name, Ref<vk::ShaderModule> vertexModule,
                 Ref<vk::ShaderModule> fragShaderModule);

    vk::PipelineLayout GetPipelineLayout() const noexcept { return m_layout; }

    auto GetShaderModules() const noexcept { return m_shaderModules; }

private:
    std::array<Ref<vk::ShaderModule>, 2> m_shaderModules;
    vk::Pipeline                         pipeline;
};
} // namespace wind