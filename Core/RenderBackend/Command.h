#pragma once

#include "VulkanHeader.h"

#include "RenderResource.h"

namespace wind {
class RenderEncoder;
class ComputeEncoder;

class ComputeShader;
class RasterShader;

enum class RenderCommandQueueType : uint8_t { Copy = 0, Graphics, Compute, AsyncCompute, General };

class CommandEncoder : public RHIResource<RHIResourceType::CommandEncoder> {
public:
    CommandEncoder(RenderCommandQueueType queueType = RenderCommandQueueType::General);
    ~CommandEncoder();

    ComputeEncoder* CreateComputeEncoder();
    RenderEncoder*  CreateRenderEncoder();

    void              Reset();
    void              Begin();
    vk::CommandBuffer Finish();

protected:
    RenderCommandQueueType m_queueType;

    vk::CommandBuffer m_nativeHandle;
    vk::CommandPool   m_cmdPool;
};

class RenderEncoder : public CommandEncoder {
public:
    RenderEncoder();
    void BindGraphicsShader(const RasterShader& shader);
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
class ImmCommandEncoder : public RHIResource<RHIResourceType::CommandBuffer> {
public:
    using TaskFunc = std::function<void(const vk::CommandBuffer&)>;
    ImmCommandEncoder();
    ~ImmCommandEncoder() = default;

    void PushTask(const TaskFunc& func);
    void Submit();

private:
    vk::CommandBuffer     m_handle;
    std::vector<TaskFunc> m_tasks;
};
} // namespace wind