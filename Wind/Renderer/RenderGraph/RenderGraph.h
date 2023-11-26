#pragma once

#include "std.h"

#include "BlackBoard.h"
#include "PassNode.h"
#include "RenderGraphPass.h"
#include "RenderGraphTexture.h"
#include "ResourceNode.h"

namespace wind {
class RenderGraphPassBase;
class Swapchain;
class FrameParms;

class RenderGraph {
public:
    struct Builder {
    public:
        Builder(RenderGraph& graph, PassNode* node) : m_graph(graph), m_node(node) {}

        RenderGraphID<RenderGraphTexture> CreateTexture(const std::string&             name,
                                                        const RenderGraphTexture::Desc desc) {
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
    friend class RenderPassNode;

    template <typename ResourceType>
    RenderGraphID<ResourceType> Create(const std::string&                 name,
                                       const typename ResourceType::Desc& desc) noexcept;

    template <typename ResourceType> ResourceType* Get(RenderGraphID<ResourceType> resourceHandle) {
        if constexpr (std::is_same_v<ResourceType, RenderGraphTexture>) {
            return &m_textures[resourceHandle.m_index];
        }
        return nullptr;
    }

    Builder AddPassInternal(const std::string& name, Scope<RenderGraphPassBase> pass);
    void    Compile();

    const Swapchain* m_swapchain;

    bool m_dirty = false;

    Blackboard m_blackBoard;

    std::unordered_map<std::string, Scope<PassNode>> m_passNodes;
    std::unordered_map<std::string, Scope<ResourceNode>> m_resourceNodes;
    
    std::vector<RenderGraphTexture>  m_textures;

    FrameParms* m_currentFrameData{nullptr};
};

template <typename Data, typename Setup, typename Execute>
RenderGraphPass<Data>& RenderGraph::AddPass(const std::string& name, Setup setup, Execute&& execute,
                                            EPassType passType) {
    auto pass = scope::Create<RenderGraphPassConcrete<Data, Execute>>(
        std::forward<Execute>(execute), passType);
    Builder builder = AddPassInternal(name, std::move(pass));
    setup(builder, const_cast<Data&>(pass->GetData()));
    return *pass;
}

template <typename ResourceType>
RenderGraphID<ResourceType> RenderGraph::Create(const std::string&                 name,
                                                const typename ResourceType::Desc& desc) noexcept {
    if constexpr (std::is_same_v<ResourceType, RenderGraphTexture>) {
        RenderGraphHandle handle;
        if(m_resourceNodes.contains(name)) {
            handle = m_resourceNodes[name]->resourceHandle;
        } else {
            handle = RenderGraphHandle(m_textures.size());
            m_resourceNodes[name] = scope::Create<ResourceNode>(*this, handle);
            m_textures.push_back(ResourceType(desc));
        }

        return RenderGraphID<ResourceType>(handle);
    }

    return {};
}
} // namespace wind