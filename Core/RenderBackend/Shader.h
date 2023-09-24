#pragma once

#include "std.h"

#include "RenderBackend/RenderResource.h"
#include "VulkanHeader.h"

namespace wind {
class Shader : public RenderResource<RenderResourceType::Shader> {
public:
    enum class ShaderTag : uint8_t {
        ComputeShader = 0,
        RasterShader,
    };
    
private:
    vk::PipelineLayout m_layout;
    vk::Pipeline       m_pipeline;
};
} // namespace wind