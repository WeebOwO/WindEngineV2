#pragma once

#include "std.h"

#include "RenderPass.h"

namespace wind {
class ResourceNode;
class RenderGraph;

class PassNode : public RenderGraphPassExecutor {
protected:
    PassNode(RenderGraph& rg) : renderGraph(rg) {}

    std::unordered_set<std::string> declareResources;
    RenderGraph& renderGraph;

public:
    
};
} // namespace wind