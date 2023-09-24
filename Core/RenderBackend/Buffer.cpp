#include "Buffer.h"

namespace wind {
GPUBuffer::GPUBuffer(u32 byteSize, vk::BufferUsageFlags usageFlags,
                     const VmaAllocationCreateInfo& AllocationCreateInfo)
    : m_byteSize(byteSize) {

    vk::BufferCreateInfo BufferCreateInfo = {
        .size = byteSize, .usage = usageFlags, .sharingMode = vk::SharingMode::eExclusive};
    m_buffer = device.AllocateBuffer(BufferCreateInfo, AllocationCreateInfo);
}

GPUBuffer::~GPUBuffer() {
    device.DeAllocateBuffer(m_buffer);
}

UploadBuffer::UploadBuffer(u32 byteSize)
    : GPUBuffer(byteSize, vk::BufferUsageFlagBits::eTransferSrc,
                VmaAllocationCreateInfo{
                    .flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT |
                             VMA_ALLOCATION_CREATE_MAPPED_BIT,
                    .usage    = VMA_MEMORY_USAGE_AUTO,
                    .priority = 1.0f,
                }) {}

StorageBuffer::StorageBuffer(u32 byteSize)
    : GPUBuffer(byteSize, vk::BufferUsageFlagBits::eTransferSrc,
                VmaAllocationCreateInfo{
                    .flags    = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT,
                    .usage    = VMA_MEMORY_USAGE_AUTO,
                    .priority = 1.0f,
                }) {}

ReadBackBuffer::ReadBackBuffer(u32 byteSize)
    : GPUBuffer(byteSize, vk::BufferUsageFlagBits::eTransferSrc,
                VmaAllocationCreateInfo{
                    .flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT |
                             VMA_ALLOCATION_CREATE_MAPPED_BIT,
                    .usage    = VMA_MEMORY_USAGE_AUTO,
                    .priority = 1.0f,
                }) {}

u8* ReadBackBuffer::MapMemory() {
    auto NativeHandle = device.GetAllocator().NativeHandle();

    if (m_mapMemory == nullptr) {
        void* memory = nullptr;
        vmaMapMemory(NativeHandle, GetAllocatedBuffer().allocation, &memory);
        m_mapMemory = (u8*)memory;
    }

    return m_mapMemory;
}

void ReadBackBuffer::UnmapMemory() {
    auto NativeHandle = device.GetAllocator().NativeHandle();
    vmaUnmapMemory(NativeHandle, GetAllocatedBuffer().allocation);
}

ReadBackBuffer::~ReadBackBuffer() {
    if(m_mapMemory != nullptr) {
        UnmapMemory();
    }
}
} // namespace wind