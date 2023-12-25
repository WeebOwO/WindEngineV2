#include "MeshPass.h"

#include "Resource/Mesh.h"

namespace wind {
MeshPassType Step(MeshPassType type) {
    uint32_t origin = uint32_t(type);
    return MeshPassType(origin + 1);
}

void MeshPass::Clear() { staticMeshes.clear(); }
} // namespace wind