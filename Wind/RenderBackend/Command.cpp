#include "Command.h"

#include "std.h"

#include "Device.h"

namespace wind {
CommandEncoder::CommandEncoder(RenderCommandQueueType queueType) : m_queueType(queueType) {
    auto queueIndices = device.GetQueueIndices();
    auto vkDevice     = device.GetVkDeviceHandle();

    u32 queueIndex = queueType == RenderCommandQueueType::Compute
                         ? queueIndices.computeQueueIndex.value()
                         : queueIndices.graphicsQueueIndex.value();

    vk::CommandPoolCreateInfo poolCreateInfo{.queueFamilyIndex = queueIndex};

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

void ImmCommandEncoder::PushTask(const TaskFunc& func) { m_tasks.push_back(func); }

void ImmCommandEncoder::Submit() {
    for (const auto& func : m_tasks) {
        func(m_handle);
    }
    // this submit may cause gpu cpu stall
    device.SubmitBackUpCommandBuffer(m_handle);
}
} // namespace wind