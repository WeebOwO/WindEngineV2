#include "Buffer.h"

namespace wind {
GPUBuffer::GPUBuffer(u32 byteSize, vk::BufferUsageFlags usageFlags,
                     const VmaAllocationCreateInfo& AllocationCreateInfo)
    : m_byteSize(byteSize) {
    auto                 allocator        = device.GetAllocator();
    vk::BufferCreateInfo BufferCreateInfo = {
        .size = byteSize, .usage = usageFlags, .sharingMode = vk::SharingMode::eExclusive};
    m_buffer = allocator.AllocateBuffer(BufferCreateInfo, AllocationCreateInfo);
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
} // namespace wind