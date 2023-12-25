#pragma once

#include "std.h"

#include "Allocator.h"
#include "RHIResource.h"
#include "VulkanHeader.h"

namespace wind {
class GPUBuffer : public RHIResource<RHIResourceType::Buffer> {
public:
    GPUBuffer(uint32_t byteSize, vk::BufferUsageFlags usageFlags,
              const VmaAllocationCreateInfo& AllocationCreateInfo) noexcept;
    ~GPUBuffer();

    GPUBuffer(const GPUBuffer& other)            = delete;
    GPUBuffer& operator=(const GPUBuffer& other) = delete;

    [[nodiscard]] auto GetNativeHandle() const { return m_buffer.buffer; }
    [[nodiscard]] auto GetByteSize() const { return m_byteSize; }
    [[nodiscard]] auto GetAllocation() { return m_buffer.allocation; }

protected:
    auto GetAllocatedBuffer() { return m_buffer; }

private:
    uint32_t             m_byteSize;
    AllocatedBuffer m_buffer;
};

class UploadBuffer : public GPUBuffer {
public:
    UploadBuffer(uint32_t                  byteSize,
                 vk::BufferUsageFlags usageFlags = vk::BufferUsageFlagBits::eTransferSrc);
    ~UploadBuffer();

    void  WriteData(void* data, uint32_t size, uint32_t offset = 0);
    void* MapMemory();

private:
    void  UnmapMemory();
    void* m_mapMemory{nullptr};
};

class DeviceBuffer : public GPUBuffer {
public:
    DeviceBuffer(uint32_t byteSize, vk::BufferUsageFlags usageFlags);
};

class ReadBackBuffer : public GPUBuffer {
public:
    ReadBackBuffer(uint32_t byteSize, vk::BufferUsageFlags usageFlags);
    ~ReadBackBuffer();

    void* MapMemory();

private:
    void UnmapMemory();

    void* m_mapMemory{nullptr};
};

class PushBuffer : public GPUBuffer {
public:
    PushBuffer(uint32_t buffersize, vk::BufferUsageFlags usage, VmaMemoryUsage vmaMemoryUsage);

    template <typename T> uint32_t Push(T& data);

    uint32_t  Push(void* data, uint32_t size);
    void Reset();

private:
    uint32_t m_align;
    uint32_t m_currentOffset;

    uint8_t* m_mapMemory{nullptr};
};
} // namespace wind

namespace wind::utils {
uint32_t PadUniformBufferSize(uint32_t originSize);
}