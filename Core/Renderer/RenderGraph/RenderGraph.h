#pragma once

#include "std.h"

#include "RenderBackend/Command.h"
#include "RenderBackend/Texture.h"
#include "RenderBackend/VulkanHeader.h"

namespace wind {
class RenderGraphPass;
class RenderGraph {
public:
    RenderGraphPass& AddPass(const std::string& passName);

    void SetBackBufferName(const std::string& backBufferName);
    void ImportExtenernalTexture();

private:
    Ref<GPUDevice>               m_device;
    std::vector<RenderGraphPass> m_graphPasses;
};
} // namespace wind