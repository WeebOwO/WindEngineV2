#pragma once

#include "Shader.h"

namespace wind {
class ShaderMap;

class RasterShader final : public Shader {
public:
    friend class GraphicsPipelineBuilder;

    RasterShader(const std::string& name, Ref<vk::ShaderModule> vertexModule,
                 Ref<vk::ShaderModule> fragModule) noexcept;

    vk::PipelineLayout GetPipelineLayout() const noexcept { return m_layout; }

    auto GetShaderModules() const noexcept { return m_shaderModules; }

    Ref<RasterShader> Compile(ShaderMap& shaderMap, const std::string& vertexFilePath,
                              const std::string& fragfilePath);

private:
    std::array<Ref<vk::ShaderModule>, 2> m_shaderModules;
    vk::Pipeline                         pipeline;
};
} // namespace wind