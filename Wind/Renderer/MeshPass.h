#pragma once

#include "std.h"

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
    using PipelineStateId = u64;
    PipelineStateId pipelineId;
    DrawMesh        drawMesh;
};

struct MeshPass {
    MeshPassType             type;
    std::vector<StaticMesh*> meshes;
};

struct MeshProcesser {
    virtual void Process(MeshPass& meshPass);
};

MeshPassType Step(MeshPassType type);
} // namespace wind