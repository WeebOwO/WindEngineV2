#pragma once

#include <memory>

#include "RenderBackend/Device.h"
#include "VulkanHeader.h"

namespace wind {
// one command encoder to one thread recording

class RenderPassEncoder {
public:

private:

};

class CommandEncoder {
public:
    
private:
    vk::CommandBuffer m_handle;
    vk::CommandPool   m_cmdPool;
};

} // namespace wind