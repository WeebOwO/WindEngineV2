#include "Shader.h"

#include <spirv_cross/spirv_glsl.hpp>

namespace wind {
void Shader::CollectMetaData(const std::vector<u32>& spirvCode, vk::ShaderStageFlags flag) {
    spirv_cross::CompilerGLSL    compiler(spirvCode);
    spirv_cross::ShaderResources resources = compiler.get_shader_resources();

    auto collectResource = [&](auto resource, vk::DescriptorType descriptorType) {
        if (m_bindings.find(resource.name) == m_bindings.end()) {
            std::string_view resourceName = resource.name;
            uint32_t set     = compiler.get_decoration(resource.id, spv::DecorationDescriptorSet);
            uint32_t binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
            const spirv_cross::SPIRType& type          = compiler.get_type(resource.type_id);
            uint32_t                     typeArraySize = type.array.size();
            uint32_t                     count         = typeArraySize == 0 ? 1 : type.array[0];
            ShaderBinding                metaData{set, binding, count, descriptorType, flag};
            m_bindings[resource.name] = metaData;
        } else {
            m_bindings[resource.name].shaderStageFlag |= flag;
        }
    };

    for (auto& resource : resources.uniform_buffers) {
        collectResource(resource, vk::DescriptorType::eUniformBuffer);
    }

    for (auto& resource : resources.sampled_images) {
        collectResource(resource, vk::DescriptorType::eCombinedImageSampler);
    }

    for (auto& resource : resources.separate_samplers) {
        collectResource(resource, vk::DescriptorType::eSampler);
    }

    for (auto& resource : resources.separate_images) {
        collectResource(resource, vk::DescriptorType::eSampledImage);
    }

    for (auto& resource : resources.storage_buffers) {
        collectResource(resource, vk::DescriptorType::eStorageBuffer);
    }

    for (const auto& resource : resources.push_constant_buffers) {
        std::string_view             resourceName = resource.name;
        const spirv_cross::SPIRType& type         = compiler.get_type(resource.type_id);
        uint32_t                     size         = compiler.get_declared_struct_size(type);
        if (!m_pushConstantMeta.has_value()) {
            PushConstantBinding meta{size, 0, flag};
            m_pushConstantMeta = std::optional<PushConstantBinding>(meta);
        } else {
            m_pushConstantMeta->shadeshaderStageFlag |= flag;
        }
    }
}
} // namespace wind