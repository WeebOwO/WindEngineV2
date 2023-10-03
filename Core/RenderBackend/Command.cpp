#include "Command.h"

#include "ComputeShader.h"

namespace wind {
CommandEncoder::CommandEncoder(RenderCommandQueueType queueType) : QueueType(queueType) {
    auto queueIndices = device.GetQueueIndices();
    auto vkDevice     = device.GetVkDeviceHandle();

    u32                       queueIndex = queueType == RenderCommandQueueType::Compute
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

vk::CommandBuffer CommandEncoder::BeginComputePass(const ComputeShader& computeShader) {
    vk::CommandBufferBeginInfo beginInfo{
        .flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit,
    };
    m_nativeHandle.begin(beginInfo);
    computeShader.Bind(m_nativeHandle);
    return m_nativeHandle;
}

} // namespace wind