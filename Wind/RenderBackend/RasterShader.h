#pragma once

#include "Shader.h"

namespace wind {
class ShaderMap;

class RasterShader final : public Shader {
public:
    friend class GraphicsPipelineBuilder;

    RasterShader(const std::string& name, const vk::ShaderModule& vertexModule,
                 const vk::ShaderModule& fragModule) noexcept;

    vk::PipelineLayout GetPipelineLayout() const noexcept { return m_layout; }

    auto GetShaderModules() const noexcept { return m_shaderModules; }

    static Ref<RasterShader> Create(const std::string& debugName, const std::string& vertexFilePath,
                                    const std::string& fragfilePath);

private:
    std::array<vk::ShaderModule, 2> m_shaderModules;
    vk::Pipeline                    pipeline;
};
} // namespace wind