#include "Command.h"

#include "std.h"
#include "ComputeShader.h"

namespace wind {
CommandEncoder::CommandEncoder(RenderCommandQueueType queueType) : m_queueType(queueType) {
    auto queueIndices = device.GetQueueIndices();
    auto vkDevice     = device.GetVkDeviceHandle();

    u32 queueIndex = queueType == RenderCommandQueueType::Compute
                         ? queueIndices.computeQueueIndex.value()
                         : queueIndices.graphicsQueueIndex.value();

    vk::CommandPoolCreateInfo poolCreateInfo{.flags =
                                                 vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
                                             .queueFamilyIndex = queueIndex};

    m_cmdPool = vkDevice.createCommandPool(poolCreateInfo);

    vk::CommandBufferAllocateInfo allocateInfo{
        .commandPool        = m_cmdPool,
        .level              = vk::CommandBufferLevel::ePrimary,
        .commandBufferCount = 1,
    };

    m_nativeHandle = vkDevice.allocateCommandBuffers(allocateInfo).front();
}

CommandEncoder::~CommandEncoder() {
    // wait command to finish job
    auto vkDevice = device.GetVkDeviceHandle();
    vkDevice.waitIdle();
    vkDevice.destroyCommandPool(m_cmdPool);
}

ComputeEncoder* CommandEncoder::CreateComputeEncoder() {
    assert(m_queueType == RenderCommandQueueType::Compute || m_queueType == RenderCommandQueueType::AsyncCompute);
    return static_cast<ComputeEncoder*>(this);
}

RenderEncoder* CommandEncoder::CreateRenderEncoder() {
    assert(m_queueType == RenderCommandQueueType::Graphics);
    return static_cast<RenderEncoder*>(this);
}

// reset the whole command pool, this is faster than reset the commandbuffer in multithread context
// https://github.com/KhronosGroup/Vulkan-Samples/blob/main/samples/performance/command_buffer_usage/README.adoc
void CommandEncoder::Reset() { device.GetVkDeviceHandle().resetCommandPool(m_cmdPool); }

void CommandEncoder::Begin() {
    vk::CommandBufferBeginInfo beginInfo{.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit};
    m_nativeHandle.begin(beginInfo);
}

vk::CommandBuffer CommandEncoder::Finish() {
    m_nativeHandle.end();
    return m_nativeHandle;
}

ImmCommandEncoder::ImmCommandEncoder() {
    m_handle = device.GetBackUpCommandBuffer();
    vk::CommandBufferBeginInfo beginInfo{.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit};
    m_handle.begin(beginInfo);
}

// Compute Encoder part
ComputeEncoder::ComputeEncoder(bool isAsync)
    : CommandEncoder(isAsync ? RenderCommandQueueType::AsyncCompute
                             : RenderCommandQueueType::Compute) {}

void ComputeEncoder::Dispatch(u32 x, u32 y, u32 z) { m_nativeHandle.dispatch(x, y, z); }

void ComputeEncoder::BindComputShader(const ComputeShader& computeShader) {
    computeShader.BindCommandBuffer(m_nativeHandle);
}
// Render Encoder part
RenderEncoder::RenderEncoder() : CommandEncoder(RenderCommandQueueType::Graphics) {}

void RenderEncoder::BeginRenderPass(const vk::RenderPassBeginInfo& renderPassBeginInfo) {
    m_nativeHandle.beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);
}

void RenderEncoder::EndRenderPass() { m_nativeHandle.endRenderPass(); }
void ImmCommandEncoder::PushTask(const TaskFunc& func) { m_tasks.push_back(func); }

void ImmCommandEncoder::Submit() {
    for (const auto& func : m_tasks) {
        func(m_handle);
    }
    // this submit may cause gpu cpu stall
    device.SubmitBackUpCommandBuffer(m_handle);
}
} // namespace wind