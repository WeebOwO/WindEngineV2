#include "MeshPass.h"

#include "Resource/Mesh.h"

namespace wind {
MeshPassType Step(MeshPassType type) {
    u32 origin = u32(type);
    return MeshPassType(origin + 1);
}

void MeshPass::Clear() { staticMeshes.clear(); }
} // namespace wind