#include "Backend.h"

#include <iostream>
#include <memory>

namespace wind {
std::shared_ptr<Backend> Backend::s_instance = nullptr;

Backend::Backend() { 
    m_device = std::make_unique<GPUDevice>(); 
}

void Backend::Init() { s_instance = std::make_shared<Backend>(); }
} // namespace wind