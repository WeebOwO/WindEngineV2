#pragma once

#include "VulkanHeader.h"

namespace wind {

enum class ImageOptions : uint8_t {
    Default = 1 << 0,
    MipMap = 1 << 1,
    CubeMap = 1 << 2
};

struct TextureDesc {
    uint32_t width;
    uint32_t height;
    vk::Format format;
    ImageOptions createOptions = ImageOptions::Default;
};

struct GPUTexture {
    vk::Image     handle;
    vk::ImageView imageView;
};
} // namespace wind