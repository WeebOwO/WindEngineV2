#pragma once

#include "PassNode.h"

namespace wind {
class ResourceRegistry {
public:
    ResourceRegistry(RenderGraph& rg, PassNode* node) : m_renderGraph(rg), m_passNode(node) {}

    vk::RenderingInfo GetRenderingInfo() const noexcept;
    vk::RenderingInfo GetPresentRenderingInfo() const noexcept;
    
private:
    PassNode*    m_passNode;
    RenderGraph& m_renderGraph;
};
} // namespace winds
