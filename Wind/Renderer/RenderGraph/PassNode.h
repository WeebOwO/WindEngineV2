#pragma once

#include "std.h"

#include "RenderGraphPass.h"

namespace wind {
class ResourceNode;
class RenderGraph;

class PassNode {
public:
    PassNode(RenderGraph& rg) : renderGraph(rg) {}
    virtual void Execute() noexcept {}

protected:
    std::unordered_set<std::string> declareResources;
    RenderGraph&                    renderGraph;
};

class RenderPassNode : public PassNode {
public:
    struct RenderPassData {
        
    };

    RenderPassNode(RenderGraph& rg, const std::string& name, Scope<RenderGraphPassBase> pass);

private:
    std::string                m_debugName;
    Scope<RenderGraphPassBase> m_passBase;
};
} // namespace wind