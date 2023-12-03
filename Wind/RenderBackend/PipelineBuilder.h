#pragma once

#include "std.h"

#include "Renderer/Material.h"
#include "VulkanHeader.h"

namespace wind {
class RasterShader;

enum EVertexType { NoVertex, StaticMesh };

// todo : add more options for different shader
class PipelineBuilder {
public:
    static constexpr vk::ColorComponentFlags BLEND_ALL_FLAG =
        vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
        vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;

    PipelineBuilder& SetInputAssemblyState(vk::PrimitiveTopology topology,
                                           bool                  primitiveRestartEnable);
    PipelineBuilder& SetVertexType(EVertexType vertexType); // will change this in future
    PipelineBuilder& SetRasterizationState(vk::PolygonMode polygonMode, vk::CullModeFlags cullMode,
                                           vk::FrontFace frontFace);
    PipelineBuilder& SetRenderState(const Material::Desc& blendMode, u32 mrtCount = 1); // this part care about blend and 
  
    vk::Pipeline Build();

private:
    void SetShaderState(const RasterShader& shader);
    
    vk::PipelineInputAssemblyStateCreateInfo m_inputAssemblyStateInfo{};
    vk::PipelineVertexInputStateCreateInfo   m_inputStateCreateInfo{};
    vk::PipelineRasterizationStateCreateInfo m_rasterizationStateInfo{};
    vk::PipelineColorBlendStateCreateInfo    m_blendStateInfo{};
    vk::PipelineDepthStencilStateCreateInfo  m_depthStencilState{};
    vk::PipelineLayout                       m_layout;

    std::vector<vk::PipelineColorBlendAttachmentState> m_attachmentState;
    std::array<vk::PipelineShaderStageCreateInfo, 2>   m_shaderStages;
};
} // namespace wind