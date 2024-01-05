#pragma once

#include "std.h"

#include "RenderGraphID.h"

#include "Resource/RenderResource.h"

namespace wind {
class PassNode;
class RenderGraph;

enum RenderGraphResourceType : uint8_t
{
    None = 0,
    Buffer,
    Texture,
};
    
struct RenderGraphResource : public RenderResource {
    RenderGraphResourceType type;
    explicit RenderGraphResource(RenderGraphResourceType type) : type(type) {}
};
} // namespace wind