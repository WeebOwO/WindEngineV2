#pragma once

#include <memory>

#include "VulkanHeader.h"
#include "Device.h"

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

enum class PipelineType { None = 0, Graphic, Compute };

struct PipelineDesc {
    PrimitiveTopology    topology      = PrimitiveTopology::Triangles;
    DepthCompareOperator depthOperator = DepthCompareOperator::LessOrEqual;
};

template<PipelineType pipelineType>
class Pipeline {    
public:
    
private:
    PipelineDesc          m_pipelineDesc;

    vk::PipelineBindPoint m_bindPoint;
    vk::PipelineLayout    m_pipelineLayout;
};

} // namespace wind