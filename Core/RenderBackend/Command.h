#pragma once

#include "VulkanHeader.h"
#include "RenderBackend/RenderResource.h"

namespace wind {

// fwd def
class ComputeShader;
class RasterShader;

// render queue type def
enum class RenderCommandQueueType : uint8_t {
    None     = 0x00,
    Copy     = 0x01,
    Compute  = 0x02,
    Graphics = 0x04,
    All      = Copy | Compute | Graphics
};

class CommandEncoder : public RenderResource<RenderResourceType::CommandPool> {
public:
    CommandEncoder(RenderCommandQueueType queueType = RenderCommandQueueType::All);
    ~CommandEncoder();

    vk::CommandBuffer BeginComputePass(const ComputeShader& computeShader);
    
    vk::CommandBuffer GetNativeHandle() const { return m_nativeHandle; }

    RenderCommandQueueType QueueType;

private:
    vk::CommandBuffer m_nativeHandle;
    vk::CommandPool   m_cmdPool;
};
} // namespace wind