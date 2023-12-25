#pragma once

#include "VulkanHeader.h"

#include "RHIResource.h"

namespace wind {
class ComputeShader;
class RasterShader;

enum class RenderCommandQueueType : uint8_t { Copy = 0, Graphics, Compute, AsyncCompute, General };

class CommandEncoder : public RHIResource<RHIResourceType::CommandEncoder> {
public:
    CommandEncoder(RenderCommandQueueType queueType = RenderCommandQueueType::General);
    ~CommandEncoder();

    void              Begin();
    void              Reset();
    vk::CommandBuffer Finish();

    // renderpart
    void BindGraphicsShader(const RasterShader& shader);

    void BeginRenderPass(const vk::RenderPassBeginInfo& renderPassBeginInfo);
    void EndRenderPass();
    void BindPSO(const vk::Pipeline& pipeline);
    void BindVertexBuffer();
    void Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance);
    void DrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, uint32_t vertexOffset,
                     uint32_t firstInstance);

    void BindVertexBuffer(uint32_t firstBinding, uint32_t bindingCount, const vk::Buffer& buffer,
                          vk::DeviceSize offset);
    void BindIndexBuffer(const vk::Buffer& buffer, vk::DeviceSize offset, vk::IndexType indexType);

    void SetViewport(const vk::Viewport& viewport);
    void SetScissor(int offsetx, int offsety, uint32_t width, uint32_t height);

    // layout transfer
    void TransferImageLayout(const vk::Image& image, vk::AccessFlags srcMask,
                             vk::AccessFlags dstMask, vk::ImageLayout oldlayout,
                             vk::ImageLayout newLayout, vk::PipelineStageFlags srcFlags,
                             vk::PipelineStageFlags           dstFlags,
                             const vk::ImageSubresourceRange& subRange); // this is verbose version

    void TransferImageLayout(const vk::Image& image, vk::ImageLayout oldLayout, vk::ImageLayout newLayout); // deduce version
 
    // dynamic rendering
    void BeginRendering(const vk::RenderingInfo& renderingInfo);
    void EndRendering();

    // uipart
    void RenderUI();

    // compute 
protected:
    RenderCommandQueueType m_queueType;

    vk::CommandBuffer m_nativeHandle;
    vk::CommandPool   m_cmdPool;
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