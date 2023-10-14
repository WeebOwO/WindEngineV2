#pragma once

#include "std.h"

#include "RenderBackend/Command.h"
#include "RenderGraphResource.h"

namespace wind {
class RenderGraph;

class RenderGraphPass {
public:
    using StencilClearCallBack     = std::function<void(vk::ClearDepthStencilValue& value)>;
    using RenderColorClearCallBack = std::function<void(vk::ClearDepthStencilValue& value)>;
    using ExecCallBack = std::function<void>(void());

    RenderGraphPass(RenderGraph* renderGraph, u32 index, RenderCommandQueueType type);

    void AddColorOuput(const std::string& resourceName, const AttachmentDesc& desc);
    void AddTextureInput();

private:
    RenderGraph*           m_refGraph; // maybe weak_ptr?
    RenderCommandQueueType m_queueType;
};
}; // namespace wind