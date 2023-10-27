#include "RasterShader.h"

#include "Renderer/Material.h"

namespace wind {
RasterShader::RasterShader(const std::string& name, const std::vector<u32>& vertexCode, const std::vector<u32>& fragcode) {
    SetShaderName(name);
    
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

} // namespace wind