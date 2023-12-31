#include "Mesh.h"

#include "Backend/Buffer.h"
#include "Backend/Command.h"

namespace wind
{
    void StaticMesh::InitRHI()
    {
        uint32_t vertexBufferByteSize = sizeof(StaticMeshVertexFactory::Vertex) * meshSource.vertices.size();
        uint32_t indexBufferByteSize  = sizeof(StaticMeshVertexFactory::Index) * meshSource.indices.size();

        UploadBuffer vertexUploadBuffer(vertexBufferByteSize);
        UploadBuffer indexUploadBuffer(indexBufferByteSize);

        meshSource.vertexBuffer = ref::Create<DeviceBuffer>(
            vertexBufferByteSize, vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst);
        meshSource.indexBuffer = ref::Create<DeviceBuffer>(
            indexBufferByteSize, vk::BufferUsageFlagBits::eIndexBuffer | vk::BufferUsageFlagBits::eTransferDst);

        vertexUploadBuffer.WriteData(meshSource.vertices.data(), vertexBufferByteSize, 0);
        indexUploadBuffer.WriteData(meshSource.indices.data(), indexBufferByteSize, 0);

        ImmCommandBuffer copyEncoder;

        copyEncoder.PushTask([&](const vk::CommandBuffer& command) {
            vk::BufferCopy bufferCopy {.srcOffset = 0, .dstOffset = 0, .size = vertexBufferByteSize};
            command.copyBuffer(
                vertexUploadBuffer.GetNativeHandle(), meshSource.vertexBuffer->GetNativeHandle(), bufferCopy);

            bufferCopy.setSize(indexBufferByteSize);
            command.copyBuffer(
                indexUploadBuffer.GetNativeHandle(), meshSource.indexBuffer->GetNativeHandle(), bufferCopy);
        });

        copyEncoder.Submit();
    }

    void StaticMesh::ReleaseRHI()
    {
        meshSource.vertexBuffer.reset();
        meshSource.indexBuffer.reset();
    }
} // namespace wind