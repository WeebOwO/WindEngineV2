#pragma once

#include "VulkanHeader.h"

#include "RHIResource.h"

namespace wind {
class RenderEncoder;
class ComputeEncoder;

class ComputeShader;
class RasterShader;
class MeshDrawCommand;

enum class RenderCommandQueueType : uint8_t { Copy = 0, Graphics, Compute, AsyncCompute, General };

class CommandEncoder : public RHIResource<RHIResourceType::CommandEncoder> {
public:
    CommandEncoder(RenderCommandQueueType queueType = RenderCommandQueueType::General);
    ~CommandEncoder();

    void              Begin();
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
    void BindGraphicsShader(const RasterShader& shader);

    void BeginRenderPass(const vk::RenderPassBeginInfo& renderPassBeginInfo);
    void EndRenderPass();
    void BindPSO(const vk::Pipeline& pipeline);
    void BindVertexBuffer();
    void Draw(u32 vertexCount, u32 instanceCount, u32 firstVertex, u32 firstInstance);
    void DrawIndexed(u32 indexCount, u32 instanceCount, u32 firstIndex, u32 vertexOffset,
                     u32 firstInstance);

    void BindVertexBuffer(u32 firstBinding, u32 bindingCount, const vk::Buffer& buffer,
                          vk::DeviceSize offset);
    void BindIndexBuffer(const vk::Buffer& buffer, vk::DeviceSize offset, vk::IndexType indexType);

    void ExecMeshDrawCommand(const MeshDrawCommand& meshDrawCommand);

    void SetViewport(const vk::Viewport& viewport);
    void SetScissor(i32 offsetx, i32 offsety, u32 width, u32 height);

    // layout transfer
    void TransferImageLayout(const vk::Image& image, vk::AccessFlags srcMask,
                             vk::AccessFlags dstMask, vk::ImageLayout oldlayout,
                             vk::ImageLayout newLayout, vk::PipelineStageFlags srcFlags,
                             vk::PipelineStageFlags           dstFlags,
                             const vk::ImageSubresourceRange& subRange);

    // dynamic rendering
    void BeginRendering(const vk::RenderingInfo& renderingInfo);
    void EndRendering();

    // uipart
    void RenderUI();
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
    void CopyBuffer();
    void Submit();

private:
    vk::CommandBuffer     m_handle;
    std::vector<TaskFunc> m_tasks;
};
} // namespace wind