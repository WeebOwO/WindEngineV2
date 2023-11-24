#pragma once

#include "Renderer/RenderGraph/RenderPass.h"
#include "std.h"

#include "RenderBackend/Command.h"
#include "RenderBackend/Texture.h"
#include "RenderGraphResource.h"
#include "RenderPass.h"

namespace wind {
class RenderGraphPassBase;
class Swapchain;
class FrameParms;

class RenderGraph {
public:
    struct Builder {
    public:
        Builder(RenderGraph* graph) : m_graph(graph) {}

    private:
        RenderGraph* m_graph;
    };

    friend class RenderGraphPassBase;
    RenderGraph();

    template <typename Data, typename Setup, typename Execute>
    RenderGraphPass<Data>& AddPass(const std::string& name, Setup setup, Execute execute,
                                   EPassType passType);

    void SetupSwapChain(const Swapchain& swapchain);
    void SetupFrameData(FrameParms& frameData);

    void Exec();

    auto& GetBlackBoard() noexcept { return m_blackBoard; }

private:
    Builder AddPassInternal(const std::string& name, Scope<RenderGraphPassBase> pass);
    void    Compile();

    const Swapchain* m_swapchain;

    bool m_dirty = false;

    Blackboard                                                  m_blackBoard;
    std::unordered_map<std::string, Scope<RenderGraphPassBase>> m_renderPasses;

    std::string m_backBufferDebugName{"None"};
    FrameParms* m_currentFrameData{nullptr};
};

template <typename Data, typename Setup, typename Execute>
RenderGraphPass<Data>& RenderGraph::AddPass(const std::string& name, Setup setup, Execute execute,
                                            EPassType passType) {
    auto    pass    = scope::Create<RenderGraphPassConcrete<Data, Execute>>(passType, execute);
    Builder builder = AddPassInternal(name, std::move(pass));
    setup(builder, pass->GetData());
    return *pass;
}
} // namespace wind