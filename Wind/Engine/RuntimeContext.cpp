#include "RuntimeContext.h"

#include <shaderc/shaderc.hpp>

#include "Core/Log.h"
#include "ECS/JobSystem.h"
#include "Engine/RuntimeContext.h"
#include "RenderBackend/ComputeShader.h"
#include "RenderBackend/Device.h"
#include "RenderBackend/RasterShader.h"
#include "RenderBackend/Shader.h"
#include "Renderer/Material.h"
#include "Renderer/RenderGraph/RenderPassEnum.h"
#include "Resource/VertexFactory.h"

namespace wind {
RuntimeContext g_runtimeContext;

ShaderMap::ShaderMap() : m_device(g_runtimeContext.device->GetVkDeviceHandle()) {}

void ShaderMap::CacheRasterShader(Ref<RasterShader> shader) {
    m_rasterShaderCache[shader->GetShaderName()] = shader;
}

void ShaderMap::CacheComputeShader(Ref<ComputeShader> shader) {
    m_computeShaderCache[shader->GetShaderName()] = shader;
}

u64 PsoCache::CachePso(const Material& material, VertexFactoryType vertextype,
                       RenderGraphPassType graphPassType) {

    auto shadingModel = material.GetShadingModel();
    auto blendMode    = material.GetBlendMode();

    u64 shadingModelID = (u64)shadingModel;
    u64 blendModeID    = (u64)blendMode;
    u64 vertextypeID   = (u64)vertextype;
    u64 passTypeID     = (u64)graphPassType;

    u64 stateID = shadingModelID | (blendModeID << 8) | (vertextypeID << 16) | (passTypeID << 24);

    if (m_pipelineCache.contains(stateID)) {
        // if we already have the pso object
        return stateID;
    }

    // assermblyStateCreateInfo
    vk::PipelineInputAssemblyStateCreateInfo assemblyStateCreateInfo{
        .topology = vk::PrimitiveTopology::eTriangleList, .primitiveRestartEnable = false};
    // input state info
    vk::PipelineVertexInputStateCreateInfo inputStateCreateInfo;

    auto inputBindingDescriptions = StaticMeshVertexFactory::GetInputBindingDescription();
    auto inputAttributeDescriptions =
        StaticMeshVertexFactory::GetVertexInputAttributeDescriptions();
    inputStateCreateInfo.setVertexBindingDescriptions(inputBindingDescriptions)
        .setVertexAttributeDescriptions(inputAttributeDescriptions);

    // rasterization
    vk::PipelineRasterizationStateCreateInfo rasterizationStateCreateInfo;
    rasterizationStateCreateInfo.setRasterizerDiscardEnable(false)
        .setDepthClampEnable(false)
        .setCullMode(vk::CullModeFlagBits::eNone)
        .setFrontFace(vk::FrontFace::eClockwise)
        .setPolygonMode(vk::PolygonMode::eFill)
        .setLineWidth(1);

    // multiSampleState
    vk::PipelineMultisampleStateCreateInfo multisampleStateCreateInfo;
    multisampleStateCreateInfo.setSampleShadingEnable(false).setRasterizationSamples(
        vk::SampleCountFlagBits::e1);

    // blendState
    vk::PipelineColorBlendStateCreateInfo blendStateInfo;
    vk::PipelineColorBlendAttachmentState blendAttachmentState;
    blendAttachmentState.setBlendEnable(blendMode != Material::BlendMode::Opaque)
        .setColorWriteMask(vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
                           vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA);
    blendStateInfo.setAttachments(blendAttachmentState);

    // depth stencil state
    vk::PipelineDepthStencilStateCreateInfo depthStencilStateCreateInfo;
    depthStencilStateCreateInfo.setDepthTestEnable(blendMode == Material::BlendMode::Opaque)
        .setDepthWriteEnable(blendMode == Material::BlendMode::Opaque)
        .setDepthCompareOp(vk::CompareOp::eLessOrEqual);

    // viewport state
    vk::PipelineViewportStateCreateInfo viewPortStateCreateInfo;
    viewPortStateCreateInfo.setViewportCount(1).setScissorCount(1);

    std::array<vk::DynamicState, 2>    dynamicStates = {vk::DynamicState::eViewport,
                                                        vk::DynamicState::eScissor};
    vk::PipelineDynamicStateCreateInfo pipelineDynamicStateCreateInfo;
    pipelineDynamicStateCreateInfo.setDynamicStates(dynamicStates);

    // shader State
    auto vertexModule = material.GetShader()->GetVertexModule();
    auto fragModule   = material.GetShader()->GetFragModule();

    std::vector<vk::PipelineShaderStageCreateInfo> shaderStageCreateInfos(2);
    shaderStageCreateInfos[0]
        .setModule(vertexModule)
        .setStage(vk::ShaderStageFlagBits::eVertex)
        .setPName("main");

    shaderStageCreateInfos[1]
        .setModule(fragModule)
        .setStage(vk::ShaderStageFlagBits::eFragment)
        .setPName("main");

    // create Pipeline
    vk::GraphicsPipelineCreateInfo pipelineCreateInfo{
        .stageCount          = 2,
        .pStages             = shaderStageCreateInfos.data(),
        .pVertexInputState   = &inputStateCreateInfo,
        .pInputAssemblyState = &assemblyStateCreateInfo,
        .pViewportState      = &viewPortStateCreateInfo,
        .pRasterizationState = &rasterizationStateCreateInfo,
        .pMultisampleState   = &multisampleStateCreateInfo,
        .pDepthStencilState  = &depthStencilStateCreateInfo,
        .pColorBlendState    = &blendStateInfo,
        .pDynamicState       = &pipelineDynamicStateCreateInfo,
        .layout              = material.GetShader()->GetPipelineLayout(),
        .renderPass          = nullptr};

    m_pipelineCache[stateID] =
        m_device.createGraphicsPipelines(nullptr, pipelineCreateInfo).value.front();

    return stateID;
}

vk::Pipeline PsoCache::GetPso(u64 pipelineStateID) {
    if (!m_pipelineCache.contains(pipelineStateID)) {
        WIND_CORE_ERROR("{} not existed", pipelineStateID);
    }
    return m_pipelineCache[pipelineStateID];
}

PsoCache::PsoCache() : m_device(g_runtimeContext.device->GetVkDeviceHandle()) {}

void PsoCache::Destroy() {
    for (const auto& [_, pipeline] : m_pipelineCache) {
        m_device.destroyPipeline(pipeline);
    }
}

void RuntimeContext::Init() {
    // Core engine part
    Log::Init();
    JobSystem::Init();
    device = scope::Create<GPUDevice>();
    // init project path
    auto currentPath = std::filesystem::current_path();

    pathManager.projectPath = currentPath.parent_path().parent_path().parent_path().parent_path();
    pathManager.shaderPath  = pathManager.projectPath.append("Shaders");
    // init shader map
    shaderMap = scope::Create<ShaderMap>();
    shaderMap->CacheRasterShader(
        RasterShader::Create("BasePassShader", "Triangle.vert.spv", "Triangle.frag.spv"));
    // init material manager
    materialManager = scope::Create<MaterialManager>();
    materialManager->InitDefaultMaterial();

    // init pso cache
    psoCache = scope::Create<PsoCache>();
}

void RuntimeContext::Quit() {
    auto vkDevice = device->GetVkDeviceHandle();
    vkDevice.waitIdle();
    shaderMap.reset(nullptr);
    guiContext->Quit(*device);
    psoCache->Destroy();
    JobSystem::Quit();
}

void RuntimeContext::PostInit(const Window& window) {
    guiContext = scope::Create<ImGUIContext>();
    guiContext->Init(*device, window);
}

std::filesystem::path GetPath(std::filesystem::path path) { return std::filesystem::path(); }
} // namespace wind