#pragma once

#include <vk_mem_alloc.h>

#include "RenderBackend/Device.h"
#include "RenderBackend/RenderResource.h"
#include "VulkanHeader.h"

namespace wind {
enum class AccessType : u8 { None, Upload, ReadBack };

class GPUBuffer : public RenderResource<RenderResourceType::Buffer> {
public:
    GPUBuffer(u32 byteSize, vk::BufferUsageFlags usageFlags, AccessType accessType);

    GPUBuffer(const GPUBuffer& other)            = delete;
    GPUBuffer& operator=(const GPUBuffer& other) = delete;

    [[nodiscard]] auto GetNativeHandle() const { return m_buffer; }
    [[nodiscard]] auto GetByteSize() const { return m_byteSize; }
    [[nodiscard]] auto GetAllocation() { return m_allocation; }

private:
    vk::Buffer    m_buffer;
    u32           m_byteSize;
    VmaAllocation m_allocation;
};

class UploadBuffer : public GPUBuffer {
public:
    UploadBuffer(u32 byteSize, vk::BufferUsageFlags usageFlags);
};

class ReadBackBuffer : public GPUBuffer {

};

class GPUOnlyBuffer : public GPUBuffer {

};
} // namespace wind