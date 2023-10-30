#pragma once

#include "std.h"

#include "Allocator.h"
#include "RenderResource.h"
#include "VulkanHeader.h"

namespace wind {
class GPUBuffer : public RHIResource<RHIResourceType::Buffer> {
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
    UploadBuffer(u32                  byteSize,
                 vk::BufferUsageFlags usageFlags = vk::BufferUsageFlagBits::eTransferSrc);
    ~UploadBuffer();

    void  WriteData(void* data, u32 size, u32 offset = 0);
    void* MapMemory();

private:
    void  UnmapMemory();
    void* m_mapMemory;
};

class DeviceBuffer : public GPUBuffer {
public:
    DeviceBuffer(u32 byteSize, vk::BufferUsageFlags usageFlags);
};

class ReadBackBuffer : public GPUBuffer {
public:
    ReadBackBuffer(u32 byteSize, vk::BufferUsageFlags usageFlags);
    ~ReadBackBuffer();

    void* MapMemory();

private:
    void UnmapMemory();

    void* m_mapMemory{nullptr};
};

class PushBuffer : public GPUBuffer {
public:
    PushBuffer(u32 buffersize, vk::BufferUsageFlags usage, VmaMemoryUsage vmaMemoryUsage);

    template <typename T> u32 Push(T& data);

    u32  Push(void* data, u32 size);
    void Reset();

private:
    u32 m_align;
    u32 m_currentOffset;

    u8* m_mapMemory{nullptr};
};
} // namespace wind

namespace wind::utils {
u32 PadUniformBufferSize(u32 originSize);
}