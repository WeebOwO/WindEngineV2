#pragma once

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
    PrimitiveTopology    Topology      = PrimitiveTopology::Triangles;
    DepthCompareOperator DepthOperator = DepthCompareOperator::LessOrEqual;
    
    
};
} // namespace wind