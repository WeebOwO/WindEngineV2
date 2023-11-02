#include "RasterShader.h"

#include "Engine/RuntimeContext.h"
#include "RenderBackend/Device.h"
#include "Resource/Loader.h"

namespace wind {

RasterShader::RasterShader(const std::string& name, const vk::ShaderModule& vertexModule,
                           const vk::ShaderModule& fragShaderModule) noexcept
    : m_shaderModules({vertexModule, fragShaderModule}) {
    SetShaderName(name);
}

Ref<RasterShader> RasterShader::Compile(const std::string& debugName,
                                        const std::string& vertexFilePath,
                                        const std::string& fragfilePath) {
    auto& shaderMap = g_runtimeContext.shaderMap;
    auto  device    = g_runtimeContext.device->GetVkDeviceHandle();

    auto vertexSpirv = io::LoadBinary<u32>(vertexFilePath);
    auto fragSpirv   = io::LoadBinary<u32>(fragfilePath);

    vk::ShaderModuleCreateInfo vertexshaderModuleCreateInfo{
        .codeSize = vertexSpirv.size() * sizeof(u32), .pCode = vertexSpirv.data()};

    vk::ShaderModuleCreateInfo fragshaderModuleCreateInfo{
        .codeSize = vertexSpirv.size() * sizeof(u32), .pCode = vertexSpirv.data()};

    auto vertexModule = device.createShaderModule(vertexshaderModuleCreateInfo);
    auto fragModule   = device.createShaderModule(fragshaderModuleCreateInfo);

    auto shader = ref::Create<RasterShader>(debugName, vertexModule, fragModule);
    // collect all information and build the pipelinelayout
    shader->CollectMetaData(vertexSpirv, vk::ShaderStageFlagBits::eVertex);
    shader->CollectMetaData(fragSpirv, vk::ShaderStageFlagBits::eFragment);
    shader->GeneratePipelineLayout();
    
    return shader;
}
} // namespace wind