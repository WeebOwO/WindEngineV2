#pragma once

#include "std.h"

#include "RenderGraphResource.h"

#include "RenderBackend/Command.h"
#include "RenderBackend/Texture.h"

namespace wind {
class RenderGraphPass;
class Swapchain;

class RenderGraph {
public:
    friend class RenderGraphPass;

    RenderGraph(GPUDevice& device);
    RenderGraphPass& AddPass(const std::string& passName, RenderCommandQueueType type);

    void ImportTexture(const std::string& resourceName, Ref<GPUTexture2D> externalTexture);
    void ImportBackBuffer(const std::string& backBufferName);

    void SetGraphicsEncoder(Ref<CommandEncoder> encoder) { m_graphicsEncoder = encoder; }
    void SetComputeEncoder(Ref<CommandEncoder> encoder) { m_computeEncoder = encoder; };

    void SetupSwapChain(const Swapchain& swapchain, u32 imageIndex);
    void Exec();

private:
    void Compile();
    void WriteResource(const std::string& passName, const std::string& resourceName);

    GPUDevice&                                            m_device;
    std::unordered_map<std::string, Ref<RenderGraphPass>> m_renderGraphPasses;
    std::unordered_map<std::string, RenderGraphResource>  m_resources;

    Ref<CommandEncoder>            m_graphicsEncoder;
    Ref<CommandEncoder>            m_computeEncoder;
    std::vector<Ref<GPUTexture2D>> m_importTexture;

    const Swapchain* m_swapchain;
    u32              m_swapchainImageIndex;
    std::string      m_backBufferDebugName{"None"};
};
} // namespace wind