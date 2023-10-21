#pragma once

#include "Shader.h"

namespace wind {

class Material;
class RasterShader : public Shader {
public:
    friend class GraphicsPipelineBuilder;

    RasterShader(const std::string& name, const std::vector<u32>& vertexCode,
                 const std::vector<u32>& fragcode);

    vk::PipelineLayout GetPipelineLayout() const noexcept { return m_layout; }

    auto GetShaderModules() const noexcept { return m_shaderModules; }
    
private:
    std::vector<vk::ShaderModule> m_shaderModules;
    vk::Pipeline                  pipeline;
};
} // namespace wind