#pragma once

#include "std.h"
#include <glm/glm.hpp>

#include "Asset/Asset.h"

namespace wind {
class DeviceBuffer;

struct Vertex {
    glm::vec3 Position;
};

struct Index {
    u32 v1, v2, v3;
};

struct MeshDesc {
    std::vector<Vertex> vertices;
    std::vector<Index>  indices;

    Ref<DeviceBuffer> vertexBuffer;
    Ref<DeviceBuffer> indexBuffer;

    i32 materialIndex;
};
} // namespace wind