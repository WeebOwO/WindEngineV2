#pragma once

#include "VulkanHeader.h"

#include "RenderBackend/Command.h"
#include "RenderBackend/RenderResource.h"

namespace wind {
struct CompileContext : public RenderResource<RenderResourceType::CompileContext> {
    CompileContext(const vk::CommandBuffer& buffer) : m_cmdBuffer(buffer) {}
    
    void              Submit();
    vk::CommandBuffer m_cmdBuffer;
};

class RenderCompileContext : CompileContext {

};

class ComputeCompileContext : CompileContext {
public:
    void CompileCommand(const RenderCommandDispatch& command);
};
} // namespace wind