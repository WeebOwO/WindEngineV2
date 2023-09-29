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
    void CollectMetaData();
private:
    vk::PipelineBindPoint m_bindPoint;
    vk::PipelineLayout    m_layout;
};
} // namespace wind