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
    using ExecCallBack             = std::function<void>(void(CommandEncoder& encoder));

    RenderGraphPass(RenderGraph* renderGraph, u32 index, RenderCommandQueueType type);

    void AddColorOuput(const std::string& resourceName, const RenderGraphTexture::Desc& desc);
    void AddTextureInput();

private:
    StencilClearCallBack     m_stencilCallback;
    RenderColorClearCallBack m_renderColorClearCallBack;
    ExecCallBack             m_execCallBack;

    std::vector<RenderGraphTexture*> m_colorOutputs;

    RenderGraph*             m_refGraph; // maybe weak_ptr?
    RenderCommandQueueType   m_queueType;
};
}; // namespace wind