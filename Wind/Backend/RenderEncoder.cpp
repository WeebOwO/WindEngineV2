#include "Command.h"

#include "Renderer/MeshPass.h"
// imgui incldue part
#include "imgui.h"
#include "imgui_impl_vulkan.h"

namespace wind {
void CommandEncoder::BeginRenderPass(const vk::RenderPassBeginInfo& renderPassBeginInfo) {
    m_nativeHandle.beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);
}

void CommandEncoder::EndRenderPass() { m_nativeHandle.endRenderPass(); }

void CommandEncoder::BeginRendering(const vk::RenderingInfo& renderingInfo) {
    m_nativeHandle.beginRendering(renderingInfo);
}

void CommandEncoder::BindPSO(const vk::Pipeline& pipeline) {
    m_nativeHandle.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline);
}

void CommandEncoder::EndRendering() { m_nativeHandle.endRendering(); }

void CommandEncoder::Draw(u32 vertexCount, u32 instanceCount, u32 firstVertex, u32 firstInstance) {
    m_nativeHandle.draw(vertexCount, instanceCount, firstVertex, firstInstance);
}

void CommandEncoder::DrawIndexed(u32 indexCount, u32 instanceCount, u32 firstIndex,
                                 u32 vertexOffset, u32 firstInstance) {
    m_nativeHandle.drawIndexed(indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
}

void CommandEncoder::BindVertexBuffer(u32 firstBinding, u32 bindingCount, const vk::Buffer& buffer,
                                      vk::DeviceSize offset) {
    m_nativeHandle.bindVertexBuffers(firstBinding, bindingCount, &buffer, &offset);
}

void CommandEncoder::BindIndexBuffer(const vk::Buffer& buffer, vk::DeviceSize offset,
                                     vk::IndexType indexType) {
    m_nativeHandle.bindIndexBuffer(buffer, offset, indexType);
}

void CommandEncoder::SetViewport(const vk::Viewport& viewport) {
    m_nativeHandle.setViewport(0, 1, &viewport);
}

void CommandEncoder::SetScissor(i32 offsetx, i32 offsety, u32 width, u32 height) {
    vk::Rect2D scissor{.offset = {.x = offsetx, .y = offsety},
                       .extent = {.width = width, .height = height}};

    m_nativeHandle.setScissor(0, 1, &scissor);
}

void CommandEncoder::TransferImageLayout(const vk::Image& image, vk::AccessFlags srcMask,
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

void CommandEncoder::TransferImageLayout(const vk::Image& image, vk::ImageLayout oldLayout, vk::ImageLayout newLayout) {
    
}

void CommandEncoder::RenderUI() {
    ImGui::Render();
    ImDrawData* main_draw_data = ImGui::GetDrawData();
    ImGui_ImplVulkan_RenderDrawData(main_draw_data, m_nativeHandle);
}

} // namespace wind