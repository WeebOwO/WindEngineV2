#include "RuntimeContext.h"

#include <shaderc/shaderc.hpp>

#include "Core/Log.h"
#include "Engine/RuntimeContext.h"
#include "Resource/VertexFactory.h"

#include "Backend/ComputeShader.h"
#include "Backend/Device.h"
#include "Backend/PipelineBuilder.h"
#include "Backend/RasterShader.h"
#include "Backend/Shader.h"

#include "Renderer/Material.h"
#include "Renderer/RenderGraph/RenderPassEnum.h"

namespace wind
{
    RuntimeContext g_runtimeContext;

    ShaderMap::ShaderMap() : m_device(g_runtimeContext.device->GetVkDeviceHandle()) {}

    void ShaderMap::CacheRasterShader(Ref<RasterShader> shader)
    {
        m_rasterShaderCache[shader->GetShaderName()] = shader;
    }

    void ShaderMap::CacheComputeShader(Ref<ComputeShader> shader)
    {
        m_computeShaderCache[shader->GetShaderName()] = shader;
    }

    uint64_t
    PsoCache::CachePso(const Material& material, VertexFactoryType vertextype, RenderGraphPassType graphPassType)
    {

        auto shadingModel = material.GetShadingModel();
        auto blendMode    = material.GetBlendMode();

        uint64_t shadingModelID = (uint64_t)shadingModel;
        uint64_t blendModeID    = (uint64_t)blendMode;
        uint64_t vertextypeID   = (uint64_t)vertextype;
        uint64_t passTypeID     = (uint64_t)graphPassType;

        uint64_t stateID = shadingModelID | (blendModeID << 8) | (vertextypeID << 16) | (passTypeID << 24);

        if (m_pipelineCacheMaterial.contains(stateID))
        {
            // if we already have the pso object
            return stateID;
        }

        PipelineBuilder builder;

        builder.SetInputAssemblyState(vk::PrimitiveTopology::eTriangleList, false)
            .SetVertexType(EVertexType::StaticMesh)
            .SetRasterizationState(vk::PolygonMode::eFill, vk::CullModeFlagBits::eNone, vk::FrontFace::eClockwise)
            .SetRenderState(material.GetMaterialDesc());

        m_pipelineCacheMaterial[stateID] = builder.Build();

        return stateID;
    }

    vk::Pipeline PsoCache::GetPso(uint64_t pipelineStateID)
    {
        if (!m_pipelineCacheMaterial.contains(pipelineStateID))
        {
            WIND_CORE_ERROR("{} not existed", pipelineStateID);
        }
        return m_pipelineCacheMaterial[pipelineStateID];
    }

    PsoCache::PsoCache() : m_device(g_runtimeContext.device->GetVkDeviceHandle()) { CreatePredefinePSO(); }

    void PsoCache::Destroy()
    {
        for (const auto& [_, pipeline] : m_pipelineCacheMaterial)
        {
            m_device.destroyPipeline(pipeline);
        }
        for (const auto& [_, pipeline] : m_pipelineCachePredefine)
        {
            m_device.destroyPipeline(pipeline);
        }
    }

    void PsoCache::CreatePredefinePSO()
    {
        // this part use to create postprocess pso
        PipelineBuilder builder;
        auto&           shaderMap = g_runtimeContext.shaderMap;

        Material::Desc desc {.debugName    = "full_screen_composite",
                             .ShadingModel = Material::ShadingModel::UnLit,
                             .blendMode    = Material::BlendMode::Opaque,
                             .rasterShader = shaderMap->GetRasterShader("CompositeShader").get()};

        builder.SetInputAssemblyState(vk::PrimitiveTopology::eTriangleList, false)
            .SetRasterizationState(vk::PolygonMode::eFill, vk::CullModeFlagBits::eNone, vk::FrontFace::eClockwise)
            .SetVertexType(EVertexType::NoVertex)
            .SetRenderState(desc);

        m_pipelineCachePredefine[desc.debugName] = builder.Build();
    }

    void RuntimeContext::Init()
    {
        // Core engine part
        Log::Init();
        device = scope::Create<GPUDevice>();
        // init project path
        auto currentPath = std::filesystem::current_path();

        pathManager.projectPath = currentPath.parent_path().parent_path().parent_path().parent_path();
        pathManager.shaderPath  = pathManager.projectPath.append("Shaders");
        // init shader map
        // todo : change this to live reload in future
        shaderMap = scope::Create<ShaderMap>();
        shaderMap->CacheRasterShader(RasterShader::Create("BasePassShader", "Triangle.vert.spv", "Triangle.frag.spv"));
        shaderMap->CacheRasterShader(
            RasterShader::Create("CompositeShader", "FullScreen.vert.spv", "Composite.frag.spv"));

        // init material manager
        materialManager = scope::Create<MaterialManager>();
        materialManager->InitDefaultMaterial();

        // init pso cache
        psoCache = scope::Create<PsoCache>();
    }

    void RuntimeContext::Quit()
    {
        auto vkDevice = device->GetVkDeviceHandle();
        vkDevice.waitIdle();
        shaderMap.reset(nullptr);
        guiContext->Quit(*device);
        psoCache->Destroy();
    }

    void RuntimeContext::PostInit(const Window& window)
    {
        guiContext = scope::Create<ImGUIContext>();
        guiContext->Init(*device, window);
    }

    std::filesystem::path GetPath(std::filesystem::path path) { return std::filesystem::path(); }

    void RuntimeUtils::GPUWaitIdle()
    {
        assert(g_runtimeContext.device != nullptr);
        g_runtimeContext.device->GetVkDeviceHandle().waitIdle();
    }

    vk::Device RuntimeUtils::GetVulkanDevice()
    {
        assert(g_runtimeContext.device != nullptr);
        return g_runtimeContext.device->GetVkDeviceHandle();
    }

    Scene* RuntimeUtils::GetActiveScene()
    {
        assert(g_runtimeContext.activeScene != nullptr);
        return g_runtimeContext.activeScene;
    }
} // namespace wind