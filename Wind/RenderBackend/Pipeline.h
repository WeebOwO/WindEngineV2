#pragma once

#include "VulkanHeader.h"

#include "RasterShader.h"

namespace wind {
// Raster state descriptor
class Material;

struct RasterState {
    vk::CullModeFlags cullingMode;
    vk::CompareOp     depthCompareOps;
    vk::BlendOp       blendOps;
    vk::BlendFactor   factor;
    bool              colorWrite;
    bool              depthWrite;
    bool              alphaToCoverage;
    bool              inverseFrontFaces;
};

struct StencilState {};

class GraphicsPipeline {
public:
    static Ref<GraphicsPipeline> Compile(const Material& material, const RasterShader& rasterShader,
                                         vk::RenderPass& renderPass);

private:
    vk::Pipeline m_vkHandle;

    vk::PipelineShaderStageCreateInfo        m_shaderStages;
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