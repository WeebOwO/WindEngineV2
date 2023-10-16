#pragma once

#include "std.h"

#include "RenderBackend/Command.h"
#include "RenderBackend/Texture.h"

namespace wind {
class RenderGraph;

struct AttachmentInfo {
    u32        width;
    u32        height;
    vk::Format format{vk::Format::eUndefined};
    u32        samples{1};
    u32        levels{1};
    u32        layers{1};
    bool       persistent{true};
};

struct BufferInfo {
    size_t               byteSize;
    vk::BufferUsageFlags usage;
    bool                 persistent;
};

class RenderGraphPass : public RHIResource<RHIResourceType::RenderPass> {
public:
    using StencilClearCallBack     = std::function<void(vk::ClearDepthStencilValue& value)>;
    using RenderColorClearCallBack = std::function<void(vk::ClearDepthStencilValue& value)>;
    using ExecCallBack             = std::function<void(CommandEncoder& encoder)>;

    RenderGraphPass(RenderGraph& renderGraph, RenderCommandQueueType type);

    RenderGraphPass& AddColorOuput(const std::string&    resourceName,
                                   const AttachmentInfo& attachmentInfo);

    RenderGraphPass& AddDepthStencilOutput(const std::string&    resourceName,
                                           const AttachmentInfo& attachmentInfo);

    void SetExecCallBack(const ExecCallBack& callback);
    void SetStencilClearCallBack(const StencilClearCallBack& callback);
    void SetRenderColorClearCallBack(const RenderColorClearCallBack& callback);

    void Compile();
    void Exec();

private:
    bool                   m_writeDepth;
    std::string            m_debugName;
    RenderGraph&           m_renderGraph;
    RenderCommandQueueType m_passtype;

    ExecCallBack             m_execCallback;
    StencilClearCallBack     m_stencilCallback;
    RenderColorClearCallBack m_renderColorClearCallBack;

    RenderCommandQueueType m_queueType;

    vk::RenderPass m_vkHandle;

    std::vector<AttachmentInfo> m_attachmentInfos;
};
}; // namespace wind