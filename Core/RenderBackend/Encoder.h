#pragma once

#include "std.h"

#include "RenderBackend/Command.h"
#include "VulkanHeader.h"

namespace wind {

class CommandEncoder {
public:
    void BeginEncoding();

    ~CommandEncoder();

private:
    friend class GPUDevice;
    vk::CommandBuffer m_cmdBuffer;
    vk::CommandPool   m_cmdPool;
};

} // namespace wind