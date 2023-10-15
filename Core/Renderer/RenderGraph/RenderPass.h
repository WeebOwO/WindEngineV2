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
    using ExecCallBack             = std::function<void(CommandEncoder& encoder)>;

    RenderGraphPass(RenderGraph& renderGraph, u32 index, RenderCommandQueueType type);

    void AddColorOuput(const std::string& resourceName, const RenderGraphTexture::Desc& desc);
    void AddTextureInput();

    void SetExecCallBack(const ExecCallBack& callback) { m_exec = callback; }
    void SetStencilClearCallBack(const StencilClearCallBack& callback) {
        m_stencilCallback = callback;
    }
    void SetRenderColorClearCallBack(const RenderColorClearCallBack& callback) {
        m_renderColorClearCallBack = callback;
    }

private:
    ExecCallBack             m_exec;
    StencilClearCallBack     m_stencilCallback;
    RenderColorClearCallBack m_renderColorClearCallBack;

    RenderGraph&           m_refGraph;
    RenderCommandQueueType m_queueType;

    std::vector<Ref<RenderGraphResource>> m_ouputResources;
    std::vector<Ref<RenderGraphResource>> m_dependencyResources;
};
}; // namespace wind