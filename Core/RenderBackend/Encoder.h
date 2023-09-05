#pragma once

#include <memory>

#include "VulkanHeader.h"
#include "RenderBackend/Device.h"

namespace wind {
// one command encoder to one thread recording
class CommandEncoder {
public:
    CommandEncoder(GPUDevice& deivce);
private:
    vk::CommandBufferBeginInfo m_beginInfo;
    vk::UniqueCommandBuffer m_handle;
    vk::CommandPool m_commandPool;
};

} // namespace wind