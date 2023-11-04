#pragma once

#include "std.h"

#include <glm/glm.hpp>

#include "RenderBackend/VulkanHeader.h"

namespace wind {
enum class VertexFactoryType : u8 { StaicMesh = 0, Count };

struct VertexFactory {};

class StaticMeshVertexFactory : public VertexFactory {
public:
    struct Vertex {
        glm::vec3 position;
    };

    struct Index {
        u32 v1, v2, v3;
    };

    static vk::VertexInputBindingDescription                GetInputBindingDescription();
    static std::vector<vk::VertexInputAttributeDescription> GetVertexInputAttributeDescriptions();
};

class NoVertexFactory : public VertexFactory {
public:
    static vk::VertexInputBindingDescription                GetInputBindingDescription();
    static std::vector<vk::VertexInputAttributeDescription> GetVertexInputAttributeDescriptions();
};
} // namespace wind