#pragma once

#include "std.h"

#include "RenderGraphID.h"
#include "RenderGraphPass.h"
#include "RenderGraphTexture.h"

#include "Engine/RenderConfig.h"

namespace wind
{
    class RenderGraph;
    class ResourceRegistry;

    class PassNode
    {
    public:
        PassNode(RenderGraph& rg) : renderGraph(rg) {}
        virtual void Execute(ResourceRegistry& resourceRegistry, CommandBuffer& encoder) noexcept {}

        virtual void InitResources() = 0;
        
    protected:
        std::vector<RenderGraphHandle>               dependResources;
        std::vector<RenderGraphHandle>               outputResources;

        std::vector<RenderGraphHandle>               edges; // link to resources
        RenderGraph&                                 renderGraph;
    };

    class RenderPassNode : public PassNode
    {
    public:
        struct Attachments
        {
            static constexpr size_t MAX_ATTACHMENT_COUNT = RenderConfig::MRT_MAX_COUNT;
            union
            {
                RenderGraphID<RenderGraphTexture> array[MAX_ATTACHMENT_COUNT];
                struct
                {
                    RenderGraphID<RenderGraphTexture> color[MAX_ATTACHMENT_COUNT - 2];
                    RenderGraphID<RenderGraphTexture> depth;
                    RenderGraphID<RenderGraphTexture> stencil;
                };
            };
        };

        struct RenderDesc
        {
            Attachments           attchments;
            vk::Rect2D            renderArea;
            uint8_t               sample;
            vk::ClearValue        clearValue;
            vk::AttachmentLoadOp  loadop  = vk::AttachmentLoadOp::eLoad;
            vk::AttachmentStoreOp storeop = vk::AttachmentStoreOp::eStore;
        };

        RenderPassNode(RenderGraph& rg, const std::string& name, Scope<RenderGraphPassBase> pass);
        virtual void Execute(ResourceRegistry& resourceRegistry, CommandBuffer& encoder) noexcept
        {
            m_passBase->Execute(resourceRegistry, encoder);
        }

        void DeclareRenderTarget(const RenderDesc& desc);
        auto GetRenderingInfo() const { return m_renderingInfo; }

        void InitResources() override;
        
    private:
        RenderDesc                               m_renderDesc {};
        std::string                              m_debugName;
        std::vector<vk::RenderingAttachmentInfo> m_colorAttachmentInfos;
        vk::RenderingAttachmentInfo              m_depthAttachmentInfo;
        vk::RenderingAttachmentInfo              m_stencilAttachmentInfo;
        vk::RenderingInfo                        m_renderingInfo {};
        Scope<RenderGraphPassBase>               m_passBase; // this attach to execute lamda
    };
} // namespace wind