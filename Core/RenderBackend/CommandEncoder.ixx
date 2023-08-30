export module CommandEncoder;

#include <vulkan/vulkan.hpp>

namespace wind {
export class CommandBuffer {
private:
    vk::CommandBuffer m_handle;
};

export class CommandEncoder {
public:
    
private:
    vk::CommandPool m_commandPool;
};
}
