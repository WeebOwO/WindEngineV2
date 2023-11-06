#pragma once

#include "Resource/VertexFactory.h"
#include "std.h"

#include "Core/UUID.h"
#include "RenderBackend/Buffer.h"

namespace wind {
class Material;
class StaticMesh;
class MeshSource;

enum MeshPassType : u8 { BasePass = 0, Count };

template <typename T> class PerpassData {
public:
    T& operator[](MeshPassType pass) { return m_data[u32(pass)]; }

    void Clear(T&& val) {
        for (int i = 0; i < MeshPassType::Count; ++i) {
            m_data[i] = val;
        }
    }

private:
    std::array<T, MeshPassType::Count> m_data;
};

struct DrawMesh {
    bool        isMerged; // if this merged, we get the mesh from scene merged vertexBuffer
    uint32_t    firstVertex;
    uint32_t    firstIndex;
    uint32_t    indexCount;
    uint32_t    vertexCount;
    MeshSource* meshSource;
};

struct MeshDrawCommand {
    u64               pipelineID;
    DrawMesh          drawMesh;
    Material*         materialProxy;
    VertexFactoryType type;
};

struct MeshPass {
    using MaterialFilter = std::function<bool(const Material& material)>;

    void Clear();

    MeshPassType             type;
    std::vector<StaticMesh*> staticMeshes;
    MaterialFilter           filter;
};

MeshPassType Step(MeshPassType type);
} // namespace wind