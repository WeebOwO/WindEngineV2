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
};

struct StencilState {};

struct RenderState {
    RasterState  rasterState;
    StencilState stencilState = {};
};
} // namespace wind