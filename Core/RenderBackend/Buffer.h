#pragma once

#include <vk_mem_alloc.h>

#include "VulkanHeader.h"
#include "RenderBackend/Allocator.h"
#include "RenderBackend/Device.h"
#include "RenderBackend/RenderResource.h"

namespace wind {

class GPUBuffer : public RenderResource<RenderResourceType::Buffer> {
public:
    GPUBuffer(u32 byteSize, vk::BufferUsageFlags usageFlags,
              const VmaAllocationCreateInfo& AllocationCreateInfo);
    ~GPUBuffer();
    GPUBuffer(const GPUBuffer& other)            = delete;
    GPUBuffer& operator=(const GPUBuffer& other) = delete;

    [[nodiscard]] auto GetNativeHandle() const { return m_buffer.buffer; }
    [[nodiscard]] auto GetByteSize() const { return m_byteSize; }
    [[nodiscard]] auto GetAllocation() { return m_buffer.allocation; }

protected:
    auto GetAllocatedBuffer() { return m_buffer; }

private:
    u32             m_byteSize;
    AllocatedBuffer m_buffer;
};

class UploadBuffer : public GPUBuffer {
public:
    UploadBuffer(u32 byteSize);
};

class StorageBuffer : public GPUBuffer {
public:
    StorageBuffer(u32 byteSize);
};

class ReadBackBuffer : public GPUBuffer {
public:
    ReadBackBuffer(u32 byteSize);
    ~ReadBackBuffer();

    u8* MapMemory();

private:
    void UnmapMemory();

    u8* m_mapMemory {nullptr};
};
} // namespace wind