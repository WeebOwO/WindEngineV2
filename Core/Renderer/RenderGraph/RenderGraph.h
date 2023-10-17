#pragma once

#include "std.h"

#include "RenderBackend/Command.h"
#include "RenderBackend/Texture.h"

namespace wind {
class RenderGraphPass;

class RenderGraph {
public:
    RenderGraph(GPUDevice& device);
    RenderGraphPass& AddPass(const std::string& passName);

    void ImportTexture(const std::string& resourceName, Ref<GPUTexture2D> externalTexture);
    void ImportBackBuffer(const std::string& backBufferName, const vk::Framebuffer& framebuffer,
                          const vk::RenderPass& renderPass);

    void SetGraphicsEncoder(Ref<CommandEncoder> encoder) { m_graphicsEncoder = encoder; }
    void SetComputeEncoder(Ref<CommandEncoder> encoder) { m_computeEncoder = encoder; };

    void SetupSwapChain();

private:
    GPUDevice&                                       m_device;
    std::unordered_map<std::string, RenderGraphPass> m_renderGraphPasses;

    Ref<CommandEncoder>            m_graphicsEncoder;
    Ref<CommandEncoder>            m_computeEncoder;
    std::vector<Ref<GPUTexture2D>> m_importTexture;

    std::string     m_backBufferDebugName{"None"};
    vk::RenderPass  m_presentRenderPass;
    vk::Framebuffer m_backBuffer; // connect to swapchain
};
} // namespace wind