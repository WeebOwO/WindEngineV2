#pragma once

#include "Material.h"
#include "Backend/Device.h"
#include "RenderGraph/RenderPassEnum.h"
#include "Resource/VertexFactory.h"

namespace wind
{
    class PsoCache
    {
    public:
        PsoCache(GPUDevice& device, ShaderMap& shaderMap);

        uint64_t CachePso(const Material& material, VertexFactoryType vertextype, RenderGraphPassType graphPassType);
        vk::Pipeline GetPso(uint64_t pipelineStateID);
        vk::Pipeline GetPso(const std::string& name);

        void Destroy();

    private:
        friend class RuntimeContext;

        void CreatePredefinePSO(ShaderMap& shaderMap);

        vk::Device                                    m_device;
        std::unordered_map<uint64_t, vk::Pipeline>    m_pipelineCacheMaterial;
        std::unordered_map<std::string, vk::Pipeline> m_pipelineCachePredefine;
    };
} // namespace wind