#include "Buffer.h"

#include "RenderBackend/Allocator.h"

namespace wind {
GPUBuffer::GPUBuffer(u32 byteSize, vk::BufferUsageFlags usageFlags, AccessType accessType)
    : m_byteSize(byteSize) {
    auto                 allocator = device.GetAllocator();
    
    VmaAllocationCreateInfo allocationCreteInfo;
    if(accessType == AccessType::None) {
    
    }
}

} // namespace wind