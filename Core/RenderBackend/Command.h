#pragma once

#include "RenderBackend/RenderResource.h"
#include "VulkanHeader.h"


namespace wind {
// fwd def
class ComputeShader;
class RasterShader;

// render queue type def
enum class RenderCommandQueueType : uint8_t { Copy = 0, Graphics, Compute, AsyncCompute, General };

using TaskFunc = std::function<void(const vk::CommandBuffer&)>;

class RenderEncoder;
class ComputeEncoder;

class CommandEncoder : public RenderResource<RenderResourceType::CommandEncoder> {
public:
    CommandEncoder(RenderCommandQueueType queueType = RenderCommandQueueType::General);
    ~CommandEncoder();

    std::unique_ptr<RenderEncoder>  BeginRenderPass();
    std::unique_ptr<ComputeEncoder> BeginComputePass(bool isAsync = false);

    void              Reset();
    vk::CommandBuffer Finish();

private:
    void Begin();
    RenderCommandQueueType m_queueType;

    vk::CommandBuffer m_nativeHandle;
    vk::CommandPool   m_cmdPool;
};

class RenderEncoder : public CommandEncoder {
public:
    RenderEncoder();
};

class ComputeEncoder : public CommandEncoder {
public:
    ComputeEncoder(bool isAsync = false);
    void Dispatch(u32 x, u32 y, u32 z);

};

// always alloc from stack, try not to use this from heap memory
class ImmCommandEncoder : public RenderResource<RenderResourceType::CommandBuffer> {
public:
    ImmCommandEncoder();
    ~ImmCommandEncoder() = default;

    void PushTask(const TaskFunc& func);
    void Submit();

private:
    vk::CommandBuffer     m_handle;
    std::vector<TaskFunc> m_tasks;
};
} // namespace wind