#pragma once

#include "std.h"

namespace wind {
enum RenderGraphResourceType : u8 { Buffer, Texture };

struct RenderGraphResource {
    RenderGraphResourceType  type;
    std::string              resoureName;
    std::vector<std::string> producer;
    std::vector<std::string> consumer;
};

} // namespace wind