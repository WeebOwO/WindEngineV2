#pragma once

#include "std.h"
#include "RenderBackend/Shader.h"

namespace wind {

class ComputeShader : public Shader {
public:
    ComputeShader(std::span<u32> );
    
private:
    static constexpr auto bindPoint = {ShaderTag::ComputeShader};
    vk::ShaderModule m_module;
};
} // namespace wind