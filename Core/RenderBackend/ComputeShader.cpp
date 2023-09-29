#include "ComputeShader.h"

namespace wind {
ComputeShader::ComputeShader(std::span<u32> spirvCode) {
    auto                       vkDevice = device.GetVkDeviceHandle();
    vk::ShaderModuleCreateInfo shaderCreateInfo{.codeSize = sizeof(u32) * spirvCode.size(),
                                                .pCode    = spirvCode.data()};
    m_computeModule = vkDevice.createShaderModule(shaderCreateInfo);
}
} // namespace wind