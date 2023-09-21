#pragma once

#include <memory>

#include "RenderBackend/Device.h"

namespace wind {
class Backend {
public:
    static void Init();
    Backend();

    [[nodiscard]] static auto& GetGPUDevice() { return *s_instance->m_device; }
    
private:
    Backend(const Backend&)                  = delete;
    Backend& operator=(const Backend& other) = delete;

    std::unique_ptr<GPUDevice> m_device;

    static std::unique_ptr<Backend> s_instance;
};

using RenderAPI = Backend;
}; // namespace wind
