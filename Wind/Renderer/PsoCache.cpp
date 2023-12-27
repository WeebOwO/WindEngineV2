#include "PsoCache.h"

#include "Core/Log.h"
#include "Backend/PipelineBuilder.h"

namespace wind
{
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

    PsoCache::PsoCache(GPUDevice& device, ShaderMap& shaderMap) : m_device(device) { CreatePredefinePSO(shaderMap); }

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

    void PsoCache::CreatePredefinePSO(ShaderMap& shaderMap)
    {
        // this part use to create postprocess pso
        PipelineBuilder builder;
        
        Material::Desc desc {.debugName    = "full_screen_composite",
                             .ShadingModel = Material::ShadingModel::UnLit,
                             .blendMode    = Material::BlendMode::Opaque,
                             .rasterShader = shaderMap.GetRasterShader("CompositeShader").get()};

        builder.SetInputAssemblyState(vk::PrimitiveTopology::eTriangleList, false)
            .SetRasterizationState(vk::PolygonMode::eFill, vk::CullModeFlagBits::eNone, vk::FrontFace::eClockwise)
            .SetVertexType(EVertexType::NoVertex)
            .SetRenderState(desc);

        m_pipelineCachePredefine[desc.debugName] = builder.Build();
    }
} // namespace wind