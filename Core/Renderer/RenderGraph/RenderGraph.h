#pragma once

#include "RenderBackend/Command.h"
#include "std.h"

#include "RenderBackend/VulkanHeader.h"

namespace wind {
class RenderGraphPass;
class RenderGraph {
public:
    RenderGraphPass& AddPass(const std::string& passName, RenderCommandQueueType type);

private:
    
};
} // namespace wind