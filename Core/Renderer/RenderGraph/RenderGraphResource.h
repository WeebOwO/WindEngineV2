#pragma once

#include "std.h"

#include "RenderBackend/VulkanHeader.h"

namespace wind {
struct AttachmentDesc {
    u32 width, height, depth;
    vk::Format format;
};
} // namespace wind