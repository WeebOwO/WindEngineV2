#pragma once

#include "Shader.h"

namespace wind {

class Material;
class RasterShader : public Shader {
public:
    friend class GraphicsPipelineBuilder;

    RasterShader(const std::vector<u32>& vertexCode, const std::vector<u32>& fragcode);
    // pipeline

private:
    std::vector<vk::ShaderModule> m_shaderModules;
    vk::Pipeline                  m_pipeline;
};
} // namespace wind