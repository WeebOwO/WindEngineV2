#include "Command.h"

#include "imgui.h"
#include "Renderer/MeshPass.h"
#include "imgui_impl_vulkan.h"

namespace wind {
RenderEncoder::RenderEncoder() : CommandEncoder(RenderCommandQueueType::Graphics) {}

void RenderEncoder::BeginRenderPass(const vk::RenderPassBeginInfo& renderPassBeginInfo) {
    m_nativeHandle.beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);
}

void RenderEncoder::EndRenderPass() { m_nativeHandle.endRenderPass(); }

void RenderEncoder::BeginRendering(const vk::RenderingInfo& renderingInfo) {
    m_nativeHandle.beginRendering(renderingInfo);
}

void RenderEncoder::BindPSO(const vk::Pipeline& pipeline) {
    m_nativeHandle.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline);
}

void RenderEncoder::EndRendering() { m_nativeHandle.endRendering(); }

void RenderEncoder::Draw(u32 vertexCount, u32 instanceCount, u32 firstVertex, u32 firstInstance) {
    m_nativeHandle.draw(vertexCount, instanceCount, firstVertex, firstInstance);
}

void RenderEncoder::DrawIndexed(u32 indexCount, u32 instanceCount, u32 firstIndex, u32 vertexOffset,
                                u32 firstInstance) {
    m_nativeHandle.drawIndexed(indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
}

void RenderEncoder::BindVertexBuffer(u32 firstBinding, u32 bindingCount, const vk::Buffer& buffer,
                                     vk::DeviceSize offset) {
    m_nativeHandle.bindVertexBuffers(firstBinding, bindingCount, &buffer, &offset);
}

void RenderEncoder::BindIndexBuffer(const vk::Buffer& buffer, vk::DeviceSize offset,
                                    vk::IndexType indexType) {
    m_nativeHandle.bindIndexBuffer(buffer, offset, indexType);
}

void RenderEncoder::SetViewport(float width, float height, float minDepth, float maxDepth) {
    vk::Viewport viewPort{
        .width = (float)width, .height = (float)height, .minDepth = minDepth, .maxDepth = maxDepth};
    m_nativeHandle.setViewport(0, 1, &viewPort);
}

void RenderEncoder::SetScissor(i32 offsetx, i32 offsety, u32 width, u32 height) {
    vk::Rect2D scissor{.offset = {.x = offsetx, .y = offsety},
                       .extent = {.width = width, .height = height}};

    m_nativeHandle.setScissor(0, 1, &scissor);
}

void RenderEncoder::TransferImageLayout(const vk::Image& image, vk::AccessFlags srcMask,
                                        vk::AccessFlags dstMask, vk::ImageLayout oldlayout,
                                        vk::ImageLayout newLayout, vk::PipelineStageFlags srcFlags,
                                        vk::PipelineStageFlags           dstFlags,
                                        const vk::ImageSubresourceRange& subRange) {
    vk::ImageMemoryBarrier imageBarrier{
        .srcAccessMask       = srcMask,
        .dstAccessMask       = dstMask,
        .oldLayout           = oldlayout,
        .newLayout           = newLayout,
        .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .image               = image,
        .subresourceRange    = subRange,
    };

    m_nativeHandle.pipelineBarrier(srcFlags, dstFlags, {}, 0, nullptr, 0, nullptr, 1,
                                   &imageBarrier);
}

void RenderEncoder::RenderUI() {
    ImGui::Render();
    ImDrawData* main_draw_data = ImGui::GetDrawData();
    ImGui_ImplVulkan_RenderDrawData(main_draw_data, m_nativeHandle);
}
} // namespace wind