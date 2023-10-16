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

    void ImportExtenernalTexture(const std::string& resourceName);

    void SetBackBufferName(const std::string& backBufferName);
    void SetGraphicsEncoder(Ref<CommandEncoder> encoder) { m_graphicsEncoder = encoder; }
    void SetComputeEncoder(Ref<CommandEncoder> encoder) { m_computeEncoder = encoder; };

private:
    GPUDevice&                                       m_device;
    std::unordered_map<std::string, RenderGraphPass> m_renderGraphPasses;
    Ref<CommandEncoder>                              m_graphicsEncoder;
    Ref<CommandEncoder>                              m_computeEncoder;
};
} // namespace wind