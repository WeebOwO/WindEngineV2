#include "PassNode.h"

namespace wind {

RenderPassNode::RenderPassNode(RenderGraph& rg, const std::string& name,
                               Scope<RenderGraphPassBase> pass)
    : PassNode(rg), m_debugName(name), m_passBase(std::move(pass)) {}
} // namespace wind