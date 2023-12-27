#pragma once

#include "std.h"

#include "Asset/Asset.h"
#include "RenderResource.h"
#include "VertexFactory.h"


namespace wind
{
    class DeviceBuffer;
    class Material;

    struct MeshSource
    {
        std::vector<StaticMeshVertexFactory::Vertex> vertices;
        std::vector<StaticMeshVertexFactory::Index>  indices;

        Ref<DeviceBuffer> vertexBuffer;
        Ref<DeviceBuffer> indexBuffer;

        int materialIndex {-1};
    };

    class StaticMesh : public RenderResource
    {
    public:
        void InitRHI() override;
        void ReleaseRHI() override;

        MeshSource meshSource;
        Material*  material;
    };
} // namespace wind