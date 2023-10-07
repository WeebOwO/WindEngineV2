#pragma once

#include "VulkanHeader.h"

#include "RenderBackend/RasterShader.h"

namespace wind {
// use for graphics pipeline

class GraphicsPipelineBuilder {
public:
    void Build(const vk::Device& device, RasterShader& shader);

    GraphicsPipelineBuilder&
    SetBlendState(const std::span<vk::PipelineColorBlendAttachmentState> blendStates);

    GraphicsPipelineBuilder& SetRenderPass(const vk::RenderPass& renderPass);
    GraphicsPipelineBuilder& SetDepthAndSetencilState();

private:
    vk::RenderPass                 m_renderPass;
    vk::GraphicsPipelineCreateInfo m_pipelineCreateInfo;
};
} // namespace wind