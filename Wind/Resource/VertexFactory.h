#pragma once

#include "std.h"

#include <glm/glm.hpp>

#include "Backend/VulkanHeader.h"

namespace wind {
enum class VertexFactoryType : uint8_t { NoVertex = 0, StaicMesh, Count };

struct VertexFactory {};

class StaticMeshVertexFactory : public VertexFactory {
public:
    struct Vertex {
        glm::vec3 position;
    };

    struct Index {
        uint32_t v1, v2, v3;
    };

    static std::vector<vk::VertexInputBindingDescription>   GetInputBindingDescription();
    static std::vector<vk::VertexInputAttributeDescription> GetVertexInputAttributeDescriptions();
};

class NoVertexFactory : public VertexFactory {
public:
    static std::vector<vk::VertexInputBindingDescription>   GetInputBindingDescription();
    static std::vector<vk::VertexInputAttributeDescription> GetVertexInputAttributeDescriptions();
};
} // namespace wind