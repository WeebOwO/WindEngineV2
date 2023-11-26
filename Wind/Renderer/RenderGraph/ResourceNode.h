#pragma once

#include "RenderGraph.h"
#include "RenderGraphID.h"

namespace wind {
class ResourceNode {
public:
    ResourceNode(RenderGraph& rg, RenderGraphHandle handle) : renderGraph(rg), resourceHandle(handle) {}

    RenderGraph&            renderGraph;
    const RenderGraphHandle resourceHandle;
};
} // namespace wind