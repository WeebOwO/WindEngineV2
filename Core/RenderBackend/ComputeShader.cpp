#include "ComputeShader.h"

namespace wind {
ComputeShader::ComputeShader(const std::vector<u32>& spirvCode) {
    auto                       vkDevice = device.GetVkDeviceHandle();
    vk::ShaderModuleCreateInfo shaderCreateInfo{.codeSize = sizeof(u32) * spirvCode.size(),
                                                .pCode    = spirvCode.data()};
    m_computeModule = vkDevice.createShaderModule(shaderCreateInfo);

    CollectMetaData(spirvCode, vk::ShaderStageFlagBits::eCompute);
}

ComputeShader::~ComputeShader() {
    auto vkDevice = device.GetVkDeviceHandle();
    vkDevice.destroyShaderModule(m_computeModule);
}
} // namespace wind