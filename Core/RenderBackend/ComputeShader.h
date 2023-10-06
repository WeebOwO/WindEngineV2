#pragma once

#include "std.h"

#include "RenderBackend/Shader.h"

namespace wind {

class ComputeShader : public Shader {
public:
    ComputeShader(const std::vector<u32>& spirvCode);
    ComputeShader(const std::filesystem::path& filepath);
    ~ComputeShader();

    auto GetPipeline() const { return m_pipeline; }

    void Bind(const vk::CommandBuffer& cmdBuffer) const ;

private:
    static constexpr auto bindPoint = {ShaderTag::ComputeShader};
    vk::ShaderModule      m_computeModule;
    vk::Pipeline          m_pipeline;
    vk::PipelineCache     m_cache;
};
} // namespace wind