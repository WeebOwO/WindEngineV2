#pragma once

#include "VulkanHeader.h"

#include "RenderBackend/RenderResource.h"

namespace wind {
class RenderEncoder;
class ComputeEncoder;
class ComputeShader;
class RasterShader;

enum class RenderCommandQueueType : uint8_t { Copy = 0, Graphics, Compute, AsyncCompute, General };

using TaskFunc = std::function<void(const vk::CommandBuffer&)>;

class CommandEncoder : public RHIResource<RenderResourceType::CommandEncoder> {
public:
    CommandEncoder(RenderCommandQueueType queueType = RenderCommandQueueType::General);
    ~CommandEncoder();

    void Begin();
    void              Reset();
    vk::CommandBuffer Finish();

protected:

    RenderCommandQueueType m_queueType;

    vk::CommandBuffer m_nativeHandle;
    vk::CommandPool   m_cmdPool;
};

class RenderEncoder : public CommandEncoder {
public:
    RenderEncoder();
    void BeginRenderPass(const vk::RenderPassBeginInfo& renderPassBeginInfo);
    void EndRenderPass();
};

class ComputeEncoder : public CommandEncoder {
public:
    ComputeEncoder(bool isAsync = false);
    void BindComputShader(const ComputeShader& computeShader);
    void Dispatch(u32 x, u32 y, u32 z);
};

// always alloc from stack, try not to use this from heap memory
class ImmCommandEncoder : public RHIResource<RenderResourceType::CommandBuffer> {
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