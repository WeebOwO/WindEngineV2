#pragma once

#include <iostream>

namespace wind {
class GPUDevice;

struct RenderResource {
    RenderResource();
    GPUDevice& device;
};

} // namespace wind