#pragma once

#include <functional>
#include <memory>

#include "RenderBackend/Command.h"
#include "VulkanHeader.h"

namespace wind {

class CommandEncoder {
public:
    void BeginEncoding();
    
    ~CommandEncoder();
    
private:
    friend class GPUDevice;
    CommandBuffer   m_handle;
    vk::CommandPool m_cmdPool;
};

} // namespace wind