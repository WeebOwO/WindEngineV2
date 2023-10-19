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
    using StencilClearCallBack     = std::function<vk::ClearDepthStencilValue()>;
    using RenderColorClearCallBack = std::function<vk::ClearColorValue()>;
    using RenderExecCallBack       = std::function<void(RenderEncoder& encoder)>;

    RenderGraphPass(RenderGraph& renderGraph, const std::string& debugName,
                    RenderCommandQueueType type);

    RenderGraphPass& AddColorOuput(const std::string&    resourceName,
                                   const AttachmentInfo& attachmentInfo);

    RenderGraphPass& AddDepthStencilOutput(const std::string&    resourceName,
                                           const AttachmentInfo& attachmentInfo);

    RenderGraphPass& SetRenderArea(const vk::Rect2D& rect);

    void MarkWriteDepth();
    void MarkWriteBackBuffer();

    void SetRenderExecCallBack(const RenderExecCallBack& callback);
    void SetStencilClearCallBack(const StencilClearCallBack& callback);
    void SetRenderColorClearCallBack(const RenderColorClearCallBack& callback);

    bool IsWriteToDepth();
    bool IsWriteToBackBuffer();

    bool ContainsResource(const std::string& resourceName);

private:
    friend class RenderGraph;
    bool m_writeToDepth = false;
    bool m_writeToBackBuffer = false;

    struct DepthOuput {
        std::string    depthOutputName;
        AttachmentInfo attachmentInfo;
    };

    RenderGraph&           m_renderGraph;
    std::string            m_debugName;
    RenderCommandQueueType m_passtype;

    RenderExecCallBack       m_renderExecCallback{nullptr};
    StencilClearCallBack     m_stencilCallback{nullptr};
    RenderColorClearCallBack m_renderColorClearCallBack{nullptr};

    RenderCommandQueueType m_queueType;

    vk::RenderPass m_vkHandle;

    std::unordered_map<std::string, AttachmentInfo> m_colorAttachmentLUT;
    std::optional<DepthOuput>                       m_depthOutput;

    vk::Rect2D m_renderArea;
};
}; // namespace wind