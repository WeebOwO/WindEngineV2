#pragma once

#include "RenderGraphID.h"

namespace wind {
class ResourceNode {
public:
    ResourceNode(RenderGraph& rg, RenderGraphHandle handle) : renderGraph(rg), resourceHandle(handle) {}
    
    RenderGraph&            renderGraph;
    const RenderGraphHandle resourceHandle; // the index in graph render resource

    // todo: change this to index version
    std::string              producer;
    std::vector<std::string> consumer;
};
} // namespace wind