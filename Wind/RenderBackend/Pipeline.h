#pragma once

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

class GraphcisPipeline {
public:
    struct Desc {
        RasterState         rasterState;
        Ref<RasterShader>   rasterShader;
        Ref<vk::RenderPass> renderPassHandle;
    };

    static Ref<GraphcisPipeline> Create(const Desc& desc);
};
} // namespace wind