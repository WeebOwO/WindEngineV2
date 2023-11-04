#pragma once

#include "std.h"

#include "Asset/Asset.h"
#include "Interface.h"

#include "VertexFactory.h"

namespace wind {
class DeviceBuffer;
class Material;

struct MeshSource {
    std::vector<StaticMeshVertexFactory::Vertex> vertices;
    std::vector<StaticMeshVertexFactory::Index>  indices;

    Ref<DeviceBuffer> vertexBuffer;
    Ref<DeviceBuffer> indexBuffer;
};

class StaticMesh : public RenderResourceInterface {
public:
    void InitRHI() override;
    void RelaseRHI() override;

    MeshSource meshSource;
    Material*  material;
};
} // namespace wind