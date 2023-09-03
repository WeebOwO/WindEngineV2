#include "Backend.h"

#include <iostream>

namespace wind {
    std::unique_ptr<Backend> Backend::s_instance = nullptr;

    Backend::Backend() {
        m_device = std::make_unique<GPUDevice>();
    }

    void Backend::Init() {
        s_instance = std::make_unique<Backend>();
    }   
} // namespace wind