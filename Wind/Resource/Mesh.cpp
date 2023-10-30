#include "Mesh.h"

#include "RenderBackend/Buffer.h"
#include "RenderBackend/Command.h"

namespace wind {
void RawMesh::InitRHI() {
    u32 vertexBufferByteSize = sizeof(Vertex) * vertices.size();
    u32 indexBufferByteSize  = sizeof(Index) * indices.size();

    UploadBuffer uploadBuffer(vertexBufferByteSize + indexBufferByteSize);

    vertexBuffer =
        ref::Create<DeviceBuffer>(vertexBufferByteSize, vk::BufferUsageFlagBits::eVertexBuffer);
    indexBuffer =
        ref::Create<DeviceBuffer>(indexBufferByteSize, vk::BufferUsageFlagBits::eIndexBuffer);

    uploadBuffer.WriteData(vertices.data(), vertexBufferByteSize, 0);
    uploadBuffer.WriteData(indices.data(), indexBufferByteSize, vertexBufferByteSize);

    ImmCommandEncoder copyEncoder;
    
    copyEncoder.PushTask([&](const vk::CommandBuffer& command) {
        vk::BufferCopy bufferCopy {
            .srcOffset = 0,
            .dstOffset = 0,
            .size = vertexBufferByteSize
        };

        command.copyBuffer(uploadBuffer.GetNativeHandle(), vertexBuffer->GetNativeHandle(), bufferCopy);

        bufferCopy.setSrcOffset(vertexBufferByteSize);

        command.copyBuffer(uploadBuffer.GetNativeHandle(), indexBuffer->GetNativeHandle(), bufferCopy);
    });

    copyEncoder.Submit();
}

void RawMesh::RelaseRHI() {
    vertexBuffer.reset();
    indexBuffer.reset();
}
} // namespace wind