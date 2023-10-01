#include "Command.h"

namespace wind {
CompileContext::CompileContext(RenderCommandQueueType queueType) {
    auto vkDevice     = device.GetVkDeviceHandle();
    auto queueIndices = device.GetQueueIndices();

    u32 queueIndex = queueType == RenderCommandQueueType::Compute
                         ? queueIndices.computeQueueIndex.value()
                         : queueIndices.graphicsQueueIndex.value();

    vk::CommandPoolCreateInfo createInfo{.queueFamilyIndex = queueIndex};

    m_cmdPool = vkDevice.createCommandPool(createInfo);

    vk::CommandBufferAllocateInfo allocateInfo{.commandPool = m_cmdPool, .commandBufferCount = 1};

    m_handle = vkDevice.allocateCommandBuffers(allocateInfo).front();
}
} // namespace wind