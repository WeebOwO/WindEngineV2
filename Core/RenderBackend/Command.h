#pragma once

#include "RenderBackend/RenderResource.h"
#include "VulkanHeader.h"
#include <functional>


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

class CommandEncoder : public RenderResource<RenderResourceType::CommandEncoder> {
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

// always alloc from stack, try not to use this from heap memory
class ImmCommand : public RenderResource<RenderResourceType::CommandBuffer> {
public:
    using TaskFunc = std::function<void(const vk::CommandBuffer&)>;

    ImmCommand();
    ~ImmCommand() = default;

    void PushTask(const TaskFunc& func);
    void Submit();

private:
    vk::CommandBuffer     m_handle;
    std::vector<TaskFunc> m_tasks;
};
} // namespace wind