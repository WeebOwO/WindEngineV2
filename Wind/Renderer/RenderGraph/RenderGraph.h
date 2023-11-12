#pragma once

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

    void ImportTexture(const std::string& resourceName, Ref<GPUTexture2D> externalTexture);
    void ImportBackBuffer(const std::string& backBufferName);

    void SetupSwapChain(const Swapchain& swapchain);
    void SetupFrameData(FrameParms& frameData);

    void Exec();

private:
    void Compile();
    void WriteResource(const std::string& passName, const std::string& resourceName);

    bool                                                  m_dirty = false;
    std::unordered_map<std::string, Ref<RenderGraphPass>> m_renderGraphPasses;
    std::unordered_map<std::string, RenderGraphResource>  m_resources;

    std::vector<Ref<GPUTexture2D>> m_importTexture;

    const Swapchain* m_swapchain;
    std::string      m_backBufferDebugName{"None"};

    FrameParms* m_currentFrameData{nullptr};
};
} // namespace wind