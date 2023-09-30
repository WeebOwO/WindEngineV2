#pragma once

#include "VulkanHeader.h"

#include "RenderBackend/Command.h"
#include "RenderBackend/RenderResource.h"

namespace wind {
class RenderCompileContext : public RenderResource<RenderResourceType::CompileContext> {
public:
    static std::unique_ptr<RenderCompileContext> Create();

    void CompileCommand(const RenderCommandDispatch& command);
    
    void Submit();
    
private:
    vk::CommandBuffer m_cmdBuffer;
    vk::CommandPool   m_cmdPool;
};
} // namespace wind