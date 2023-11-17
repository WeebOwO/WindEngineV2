#pragma once

#include "Renderer/RenderGraph/RenderGraphResource.h"
#include "std.h"

#include "RenderBackend/Command.h"
#include "RenderBackend/Texture.h"
#include "RenderGraphResource.h"

namespace wind {
class RenderGraphPass;
class Swapchain;
class FrameParms;

class RenderGraph {
public:
    friend class RenderGraphPass;

    RenderGraph();
    RenderGraphPass* AddPass(const std::string& passName, RenderCommandQueueType type);

    void ImportBackBuffer(const std::string& backBufferName);

    void SetupSwapChain(const Swapchain& swapchain);
    void SetupFrameData(FrameParms& frameData);

    void Exec();

    auto& GetBlackBoard() noexcept { return m_blackBoard; }

private:
    void Compile();
    void WriteResource(const std::string& passName, const std::string& resourceName);

    const Swapchain* m_swapchain;

    bool                                                  m_dirty = false;
    std::unordered_map<std::string, Ref<RenderGraphPass>> m_renderGraphPasses;
    Blackboard                                            m_blackBoard;

    std::string m_backBufferDebugName{"None"};
    FrameParms* m_currentFrameData{nullptr};
};
} // namespace wind