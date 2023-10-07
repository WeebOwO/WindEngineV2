#include "RasterShader.h"

namespace wind {
RasterShader::RasterShader(const std::vector<u32>& vertexCode, const std::vector<u32>& fragcode) {
    auto vkDevice = device.GetVkDeviceHandle();

    CollectMetaData(vertexCode, vk::ShaderStageFlagBits::eVertex);
    CollectMetaData(fragcode, vk::ShaderStageFlagBits::eFragment);

    vk::ShaderModuleCreateInfo vertexModuleCreateInfo{.codeSize = sizeof(u32) * vertexCode.size(),
                                                      .pCode    = vertexCode.data()};

    vk::ShaderModuleCreateInfo fragModuleCreateInfo{.codeSize = sizeof(u32) * fragcode.size(),
                                                    .pCode    = fragcode.data()};

    m_shaderModules.push_back(vkDevice.createShaderModule(vertexModuleCreateInfo));
    m_shaderModules.push_back(vkDevice.createShaderModule(fragModuleCreateInfo));

    GeneratePipelineLayout();
}

void RasterShader::CreatePipeline(const vk::GraphicsPipelineCreateInfo& pipelineCreteInfo) {
    auto vkDevice = device.GetVkDeviceHandle();
    m_pipeline = vkDevice.createGraphicsPipeline(nullptr, pipelineCreteInfo).value;
}
} // namespace wind