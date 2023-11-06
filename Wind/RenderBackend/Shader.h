#pragma once

#include "std.h"

#include "RHIResource.h"
#include "VulkanHeader.h"

namespace wind {
class Shader : public RHIResource<RHIResourceType::Shader> {
public:
    enum class ShaderTag : uint8_t {
        ComputeShader = 0,
        RasterShader,
    };

    ~Shader();

    virtual void BindCommandBuffer(const vk::CommandBuffer& cmdBuffer) const;

    auto GetShaderName() const { return m_debugName; }

protected:
    auto SetShaderName(const std::string& name) { m_debugName = name; }
    void CollectMetaData(const std::vector<u32>& spirvCode, vk::ShaderStageFlags flag);
    void GeneratePipelineLayout();

    struct ShaderBinding {
        u32                  set;
        u32                  binding;
        u32                  count;
        vk::DescriptorType   descriptorType;
        vk::ShaderStageFlags shaderStageFlag;
    };

    struct PushConstantBinding {
        u32                  size;
        u32                  offset;
        vk::ShaderStageFlags shadeshaderStageFlag;
    };

    std::string                                    m_debugName;
    vk::PipelineLayout                             m_layout;
    std::unordered_map<std::string, ShaderBinding> m_bindings;

    std::vector<vk::PushConstantRange>                   m_pushRanges;
    std::unordered_map<std::string, PushConstantBinding> m_pushConstantBinding;

    std::vector<vk::DescriptorSetLayout> m_descriptorSetLayouts;
};
} // namespace wind