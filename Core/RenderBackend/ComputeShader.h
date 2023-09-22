#pragma once

#include "RenderBackend/Shader.h"

namespace wind {
class ComputeShader : public Shader {
public:
    ComputeShader();
private:
    static constexpr auto bindPoint = {ShaderTag::ComputeShader};
};
} // namespace wind