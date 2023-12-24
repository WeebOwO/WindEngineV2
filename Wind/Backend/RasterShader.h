#pragma once

#include "Shader.h"

namespace wind {
class ShaderMap;

class RasterShader final : public Shader {
public:
    friend class GraphicsPipelineBuilder;

    RasterShader(const std::string& name, const vk::ShaderModule& vertexModule,
                 const vk::ShaderModule& fragModule) noexcept;

    ~RasterShader();

    vk::PipelineLayout GetPipelineLayout() const noexcept { return m_layout; }

    auto GetVertexModule() const noexcept { return m_vertexModule; }
    auto GetFragModule() const noexcept { return m_fragModule; }

    static Ref<RasterShader> Create(const std::string& debugName, const std::string& vertexFilePath,
                                    const std::string& fragfilePath);

private:
    vk::ShaderModule m_vertexModule;
    vk::ShaderModule m_fragModule;
};
} // namespace wind