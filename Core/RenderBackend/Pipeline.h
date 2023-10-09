#pragma once

#include "VulkanHeader.h"

#include "RenderBackend/RasterShader.h"

namespace wind {
// use for graphics pipeline

class GraphicsPipelineBuilder {
public:
    vk::Pipeline Build(const vk::Device& device, const Material& material, const vk::RenderPass& renderPass);
    
    void SetInput();
    
private:
    vk::RenderPass m_renderPass;

    vk::GraphicsPipelineCreateInfo           m_pipelineCreateInfo;
    vk::PipelineShaderStageCreateInfo        m_ShaderStages;
    vk::PipelineVertexInputStateCreateInfo   m_vertexInputState;
    vk::PipelineInputAssemblyStateCreateInfo m_inputAssemblyState;
    vk::PipelineRasterizationStateCreateInfo m_rasterState;
    vk::PipelineMultisampleStateCreateInfo   m_multiSampleState;
    vk::PipelineDepthStencilStateCreateInfo  m_depthStencilState;
    vk::PipelineColorBlendStateCreateInfo    m_blendState;
    vk::PipelineDynamicStateCreateInfo       m_dynamicState;
    vk::PipelineViewportStateCreateInfo      m_viewPortState; // may be dynamic
};
} // namespace wind