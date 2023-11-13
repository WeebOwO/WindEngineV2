#pragma once

#include "std.h"

#include "Asset/Asset.h"
#include "VertexFactory.h"
#include "RenderResource.h"

namespace wind {
class DeviceBuffer;
class Material;

struct MeshSource {
    std::vector<StaticMeshVertexFactory::Vertex> vertices;
    std::vector<StaticMeshVertexFactory::Index>  indices;

    Ref<DeviceBuffer> vertexBuffer;
    Ref<DeviceBuffer> indexBuffer;

    i32 materialIndex{-1};
};

class StaticMesh : public RenderResource {
public:
    void InitRHI() override;
    void RelaseRHI() override;

    MeshSource meshSource;
    Material*  material;
};
} // namespace wind