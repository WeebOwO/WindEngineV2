#pragma once

#include "RenderBackend/RenderResource.h"
#include "VulkanHeader.h"

#include "RasterShader.h"

namespace wind {
// Raster state descriptor
class Material;

struct RasterState {
    vk::CullModeFlags cullingMode;
    vk::CompareOp     depthCompareOps;
    vk::BlendOp       blendOps;
    vk::BlendFactor   factor;
    bool              colorWrite;
    bool              depthWrite;
    bool              alphaToCoverage;
    bool              inverseFrontFaces;
};

struct StencilState {};

struct RenderState {
    RasterState  rasterState;
    StencilState stencilState = {};
};

class GraphcisPipeline : public RHIResource<RHIResourceType::Pipeline> {
public:
    static Ref<GraphcisPipeline> Create(const Material& material, const RasterShader& rasterShader);

private:
    vk::Pipeline m_pipelineHandle;
};
} // namespace wind