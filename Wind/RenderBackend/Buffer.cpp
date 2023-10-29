#include "Buffer.h"

#include "RenderBackend/Buffer.h"

namespace wind {
GPUBuffer::GPUBuffer(u32 byteSize, vk::BufferUsageFlags usageFlags,
                     const VmaAllocationCreateInfo& AllocationCreateInfo)
    : m_byteSize(byteSize) {

    vk::BufferCreateInfo BufferCreateInfo = {
        .size = byteSize, .usage = usageFlags, .sharingMode = vk::SharingMode::eExclusive};
    m_buffer = device.AllocateBuffer(BufferCreateInfo, AllocationCreateInfo);
}

GPUBuffer::~GPUBuffer() { device.DestroyBuffer(m_buffer); }

UploadBuffer::UploadBuffer(u32 byteSize, vk::BufferUsageFlags usageFlags)
    : GPUBuffer(byteSize, usageFlags,
                VmaAllocationCreateInfo{
                    .flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT |
                             VMA_ALLOCATION_CREATE_MAPPED_BIT,
                    .usage    = VMA_MEMORY_USAGE_AUTO,
                    .priority = 1.0f,
                }) {}

u8* UploadBuffer::MapMemory() {
    auto& nativeHandle = device.GetAllocator()->NativeHandle();
    
    if (m_mapMemory == nullptr) {
        void* memory = nullptr;
        vmaMapMemory(nativeHandle, GetAllocatedBuffer().allocation, &memory);
        m_mapMemory = (u8*)memory;
    }

    return m_mapMemory;
}

DeviceBuffer::DeviceBuffer(u32 byteSize, vk::BufferUsageFlags usageFlags)
    : GPUBuffer(byteSize, usageFlags,
                VmaAllocationCreateInfo{
                    .flags    = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT,
                    .usage    = VMA_MEMORY_USAGE_AUTO,
                    .priority = 1.0f,
                }) {}

ReadBackBuffer::ReadBackBuffer(u32 byteSize, vk::BufferUsageFlags usageFlags)
    : GPUBuffer(byteSize, usageFlags,
                VmaAllocationCreateInfo{
                    .flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT |
                             VMA_ALLOCATION_CREATE_MAPPED_BIT,
                    .usage    = VMA_MEMORY_USAGE_AUTO,
                    .priority = 1.0f,
                }) {}

u8* ReadBackBuffer::MapMemory() {
    auto& nativeHandle = device.GetAllocator()->NativeHandle();
    if (m_mapMemory == nullptr) {
        void* memory = nullptr;
        vmaMapMemory(nativeHandle, GetAllocatedBuffer().allocation, &memory);
        m_mapMemory = (u8*)memory;
    }

    return m_mapMemory;
}

UploadBuffer::~UploadBuffer() {
    if (m_mapMemory != nullptr) { UnmapMemory(); }
}

void UploadBuffer::UnmapMemory() {
    auto& nativeHandle = device.GetAllocator()->NativeHandle();
    vmaUnmapMemory(nativeHandle, GetAllocatedBuffer().allocation);
}

void ReadBackBuffer::UnmapMemory() {
    auto& nativeHandle = device.GetAllocator()->NativeHandle();
    vmaUnmapMemory(nativeHandle, GetAllocatedBuffer().allocation);
}

ReadBackBuffer::~ReadBackBuffer() {
    if (m_mapMemory != nullptr) { UnmapMemory(); }
}

PushBuffer::PushBuffer(u32 buffersize, vk::BufferUsageFlags usage, VmaMemoryUsage vmaMemoryUsage)
    : GPUBuffer(buffersize, usage, VmaAllocationCreateInfo{.usage = vmaMemoryUsage}) {}

void PushBuffer::Reset() { m_currentOffset = 0; }
} // namespace wind

namespace wind::utils {
u32 PadUniformBufferSize(u32 originSize) {
    auto limits          = Backend::GetGPUDevice().GetLimits();
    u32  minUboAlignment = limits.minUniformBufferOffsetAlignment;
    u32  alignedSize     = originSize;
    if (minUboAlignment > 0) {
        alignedSize = (alignedSize + minUboAlignment - 1) & ~(minUboAlignment - 1);
    }
    return alignedSize;
}
} // namespace wind::utils