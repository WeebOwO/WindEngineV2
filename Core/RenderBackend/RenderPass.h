#pragma ocne

#include "VulkanHeader.h" 
#include "RenderBackend/Device.h"

namespace wind {

struct RenderPassDesc {
    
};

class RenderPass {    
public:
    // create interface
    static std::unique_ptr<RenderPass> Create(GPUDevice* device, const RenderPassDesc& desc);

private:
    vk::RenderPass m_handle;
};

} // namespace wind