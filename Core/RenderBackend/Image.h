#pragma once

#include "VulkanHeader.h"

namespace wind {
struct ImageFormat {
    
};

class GPUTextureView {

};

class GPUTexture {
public:
    
private:
    vk::Image m_handle;
    vk::ImageView m_imageView;
};
} // namespace wind