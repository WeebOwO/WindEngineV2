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

    void CollectMetaData(const std::vector<u32>& spirvCode, vk::ShaderStageFlags flag);

private:
    struct ShaderBinding {
        u32                  set;
        u32                  binding;
        u32                  count;
        vk::DescriptorType   descriptorType;
        vk::ShaderStageFlags shaderStageFlag;
    };

    struct PushConstantBinding {
        uint32_t             size;
        uint32_t             offset;
        vk::ShaderStageFlags shadeshaderStageFlag;
    };

    std::unordered_map<std::string, ShaderBinding> m_bindings;

    std::optional<PushConstantBinding>  m_pushConstantMeta {std::nullopt};
    std::optional<vk::PushConstantRange> m_pushConstantRange {std::nullopt};
};
} // namespace wind