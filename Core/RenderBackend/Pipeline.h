#pragma once

#include "VulkanHeader.h"

namespace wind {
enum class PrimitiveTopology {
    None = 0,
    Points,
    Lines,
    Triangles,
    LineStrip,
    TriangleStrip,
    TriangleFan
};

enum class DepthCompareOperator {
    None = 0,
    Never,
    NotEqual,
    Less,
    LessOrEqual,
    Greater,
    GreaterOrEqual,
    Equal,
    Always,
};

struct PipelineDesc {
    PrimitiveTopology    topology      = PrimitiveTopology::Triangles;
    DepthCompareOperator depthOperator = DepthCompareOperator::LessOrEqual;  
};

struct Pipeline {
    vk::PipelineBindPoint bindPoint;
    
};



} // namespace wind