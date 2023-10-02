#pragma once

#include "std.h"

#include "VulkanHeader.h"
#include "RenderBackend/RenderResource.h"

namespace wind {
class Shader : public RenderResource<RenderResourceType::Shader> {
public:
    enum class ShaderTag : uint8_t {
        ComputeShader = 0,
        RasterShader,
    };

    ~Shader();

    void BindResource(const std::string& resourceName, const vk::DescriptorBufferInfo& bufferInfo);

protected:
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

    vk::PipelineLayout                             m_layout;
    std::unordered_map<std::string, ShaderBinding> m_bindings;

    std::vector<vk::PushConstantRange>                   m_pushRanges;
    std::unordered_map<std::string, PushConstantBinding> m_pushConstantBinding;

    std::vector<vk::DescriptorSetLayout> m_descriptorSetLayouts;
    std::vector<vk::DescriptorSet>       m_descriptorSets;
};

class ShaderCache {
    
};
} // namespace wind