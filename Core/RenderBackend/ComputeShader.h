#pragma once

#include "RenderBackend/Shader.h"
#include "std.h"

namespace wind {

class ComputeShader : public Shader {
public:
    ComputeShader(const std::vector<u32>& spirvCode);
    ~ComputeShader();

    auto GetPipeline() const { return m_pipeline; }

private:
    static constexpr auto bindPoint = {ShaderTag::ComputeShader};
    vk::ShaderModule      m_computeModule;
    vk::Pipeline          m_pipeline;
    vk::Pipeline          m_layout;
};
} // namespace wind