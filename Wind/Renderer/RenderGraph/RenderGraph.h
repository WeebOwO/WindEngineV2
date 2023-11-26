#pragma once

#include "std.h"

#include "PassNode.h"
#include "BlackBoard.h"
#include "ResourceNode.h"
#include "RenderGraphPass.h"
#include "RenderGraphTexture.h"

#include "RenderBackend/Command.h"
#include "RenderBackend/Texture.h"

namespace wind {
class RenderGraphPassBase;
class Swapchain;
class FrameParms;
class ResourceNode;

class RenderGraph {
public:
    struct Builder {
    public:
        Builder(RenderGraph& graph, PassNode* node) : m_graph(graph), m_node(node) {}

        RenderGraphID<RenderGraphTexture> CreateTexture(const std::string& name, const RenderGraphTexture::Desc desc) {
            return m_graph.Create<RenderGraphTexture>(name, desc);
        };

    private:
        RenderGraph& m_graph;
        PassNode*    m_node;
    };

    RenderGraph();

    template <typename Data, typename Setup, typename Execute>
    RenderGraphPass<Data>& AddPass(const std::string& name, Setup setup, Execute&& execute,
                                   EPassType passType);

    void SetupSwapChain(const Swapchain& swapchain);
    void SetupFrameData(FrameParms& frameData);

    void Exec();

    auto& GetBlackBoard() noexcept { return m_blackBoard; }

private:
    template<typename ResourceType>
    RenderGraphID<ResourceType> Create(const std::string& name, const typename ResourceType::Desc& desc) noexcept;

    Builder AddPassInternal(const std::string& name, Scope<RenderGraphPassBase> pass);
    void    Compile();

    const Swapchain* m_swapchain;

    bool m_dirty = false;

    Blackboard m_blackBoard;

    std::vector<Scope<PassNode>>      m_passNodes;
    std::vector<Scope<ResourceNode>>  m_resourceNodes;
    
    std::vector<RenderGraphTexture>   m_textures;
    
    std::string m_backBufferDebugName{"None"};
    FrameParms* m_currentFrameData{nullptr};
};

template <typename Data, typename Setup, typename Execute>
RenderGraphPass<Data>& RenderGraph::AddPass(const std::string& name, Setup setup, Execute&& execute,
                                            EPassType passType) {
    auto    pass    = scope::Create<RenderGraphPassConcrete<Data, Execute>>(std::forward<Execute>(execute), passType);
    Builder builder = AddPassInternal(name, std::move(pass));
    setup(builder, const_cast<Data&>(pass->GetData()));
    return *pass;
}

template<typename ResourceType>
RenderGraphID<ResourceType> RenderGraph::Create(const std::string& name, const typename ResourceType::Desc& desc) noexcept {
    if constexpr (std::is_same_v<ResourceType, RenderGraphTexture>) {
        RenderGraphHandle handle (m_textures.size());
        m_resourceNodes.push_back(scope::Create<ResourceNode>(*this, handle));
        m_textures.push_back(ResourceType(desc));
        return RenderGraphID<ResourceType>(handle);
    } 
    return {};
}
} // namespace wind