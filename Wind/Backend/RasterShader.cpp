#include "RasterShader.h"

#include "Backend/Device.h"

#include "Resource/Loader.h"

namespace wind {

RasterShader::RasterShader(const std::string& name, const vk::ShaderModule& vertexModule,
                           const vk::ShaderModule& fragShaderModule) noexcept
    : m_vertexModule(vertexModule), m_fragModule(fragShaderModule) {
    SetShaderName(name);
}

Ref<RasterShader> RasterShader::Create(const std::string& debugName,
                                       const std::string& vertexFilePath,
                                       const std::string& fragfilePath) {

    auto device = g_runtimeContext.device->GetVkDeviceHandle();

    auto vertexSpirv = io::LoadBinary<uint32_t>(vertexFilePath);
    auto fragSpirv   = io::LoadBinary<uint32_t>(fragfilePath);

    vk::ShaderModuleCreateInfo vertexshaderModuleCreateInfo{
        .codeSize = vertexSpirv.size() * sizeof(uint32_t), .pCode = vertexSpirv.data()};

    vk::ShaderModuleCreateInfo fragshaderModuleCreateInfo{
        .codeSize = fragSpirv.size() * sizeof(uint32_t), .pCode = fragSpirv.data()};

    auto vertexModule = device.createShaderModule(vertexshaderModuleCreateInfo);
    auto fragModule   = device.createShaderModule(fragshaderModuleCreateInfo);

    auto shader = ref::Create<RasterShader>(debugName, vertexModule, fragModule);
    // collect all information and build the pipelinelayout
    shader->CollectMetaData(vertexSpirv, vk::ShaderStageFlagBits::eVertex);
    shader->CollectMetaData(fragSpirv, vk::ShaderStageFlagBits::eFragment);
    shader->GeneratePipelineLayout();

    return shader;
}

RasterShader::~RasterShader() {
    auto vkDevice = device.GetVkDeviceHandle();
    vkDevice.destroyShaderModule(m_vertexModule);
    vkDevice.destroyShaderModule(m_fragModule);
}
} // namespace wind