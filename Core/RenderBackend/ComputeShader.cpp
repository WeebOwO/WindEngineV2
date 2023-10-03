#include "ComputeShader.h"

#include "Base/Log.h"

namespace wind {
ComputeShader::ComputeShader(const std::vector<u32>& spirvCode) {
    auto                       vkDevice = device.GetVkDeviceHandle();
    vk::ShaderModuleCreateInfo shaderCreateInfo{.codeSize = sizeof(u32) * spirvCode.size(),
                                                .pCode    = spirvCode.data()};
    m_computeModule = vkDevice.createShaderModule(shaderCreateInfo);

    CollectMetaData(spirvCode, vk::ShaderStageFlagBits::eCompute);
    GeneratePipelineLayout();

    vk::PipelineShaderStageCreateInfo shaderStage{
        .stage  = vk::ShaderStageFlagBits::eCompute,
        .module = m_computeModule,
        .pName  = "main",
    };

    vk::ComputePipelineCreateInfo pipelineCreateInfo{.stage = shaderStage, .layout = m_layout};

    m_pipeline = vkDevice.createComputePipeline(nullptr, pipelineCreateInfo).value;
}

void ComputeShader::Bind(const vk::CommandBuffer& cmdBuffer) const {
    cmdBuffer.bindPipeline(vk::PipelineBindPoint::eCompute, m_pipeline);
    cmdBuffer.bindDescriptorSets(vk::PipelineBindPoint::eCompute, m_layout, 0, m_descriptorSets,
                                 {});
}

ComputeShader::~ComputeShader() {
    auto vkDevice = device.GetVkDeviceHandle();
    vkDevice.waitIdle();
    vkDevice.destroyShaderModule(m_computeModule);
    vkDevice.destroyPipeline(m_pipeline);
}
} // namespace wind