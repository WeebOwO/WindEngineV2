#pragma once

#include <memory>
#include <vulkan/vulkan.hpp>

namespace wind {
// one command encoder to one thread recording
template <typename CommandType>
class CommandEncoder {
public:
    vk::CommandBuffer GetCommandBuffer();
private:
    vk::CommandBufferBeginInfo m_beginInfo;
    vk::UniqueCommandBuffer m_handle;
    vk::CommandPool m_commandPool;
};

} // namespace wind