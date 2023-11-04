#include "VertexFactory.h"

namespace wind {
vk::VertexInputBindingDescription StaticMeshVertexFactory::GetInputBindingDescription() {
    vk::VertexInputBindingDescription vertexInputBindingDescription{
        .binding   = 0,
        .stride    = sizeof(StaticMeshVertexFactory::Vertex),
        .inputRate = vk::VertexInputRate::eVertex,
    };

    return vertexInputBindingDescription;
}

std::vector<vk::VertexInputAttributeDescription>
StaticMeshVertexFactory::GetVertexInputAttributeDescriptions() {
    return {};
}

vk::VertexInputBindingDescription NoVertexFactory::GetInputBindingDescription() { return {}; }

std::vector<vk::VertexInputAttributeDescription>
NoVertexFactory::GetVertexInputAttributeDescriptions() {
    std::vector<vk::VertexInputAttributeDescription> vertexInputAttributeDescription(1);
    vertexInputAttributeDescription[0]
        .setBinding(0)
        .setLocation(0)
        .setFormat(vk::Format::eR32G32B32Sfloat)
        .setOffset(offsetof(StaticMeshVertexFactory::Vertex, position));
    return vertexInputAttributeDescription;
}
} // namespace wind