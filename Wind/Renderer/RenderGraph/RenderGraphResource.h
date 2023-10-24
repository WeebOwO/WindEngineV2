#pragma once

#include "std.h"

namespace wind {
enum RenderGraphResourceType : u8 { Buffer, Texture };

struct RenderGraphResource {
    RenderGraphResourceType         type;
    std::string                     resoureName;
    std::unordered_set<std::string> producer;
    std::unordered_set<std::string> consumer;
};
} // namespace wind