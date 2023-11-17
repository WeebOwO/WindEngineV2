#pragma once

#include "std.h"

#include "RenderGraphResource.h"
#include "RenderBackend/Command.h"
#include "RenderBackend/Texture.h"

namespace wind {
class RenderGraph;

struct DepthStencilInfo {};

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

    RenderGraphPass& DeclareRenderTarget(const std::string&    resourceName,
                                         const AttachmentInfo& attachmentInfo);

    RenderGraphPass& DeclareDepthStencil(const std::string&    resourceName,
                                         const AttachmentInfo& attachmentInfo);

    RenderGraphPass& SetRenderArea(const vk::Rect2D& rect);

    void MarkWriteBackBuffer();

    void SetRenderExecCallBack(const RenderExecCallBack& callback);
    void SetStencilClearCallBack(const StencilClearCallBack& callback);
    void SetRenderColorClearCallBack(const RenderColorClearCallBack& callback);

    bool IsWriteToDepth();
    bool IsWriteToBackBuffer();

    bool ContainsResource(const std::string& resourceName);

private:
    friend class RenderGraph;

    void Bake();

    bool m_writeToDepth      = false;
    bool m_writeToBackBuffer = false;

    struct DepthOuput {
        std::string                 depthOutputName;
        vk::RenderingAttachmentInfo attachmentInfo;
    };

    RenderGraph& m_renderGraph;
    std::string  m_debugName;

    RenderExecCallBack       m_renderExecCallback{nullptr};
    StencilClearCallBack     m_stencilCallback{nullptr};
    RenderColorClearCallBack m_renderColorClearCallBack{nullptr};

    RenderCommandQueueType m_queueType;

    vk::Rect2D m_renderArea;

    std::unordered_map<std::string, AttachmentInfo> m_renderTargets;
    std::optional<DepthOuput>                       m_depthOutput;

    vk::RenderingInfo m_renderingInfo;  
};

}; // namespace wind