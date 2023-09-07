#pragma once

#include <memory>

#include "RenderBackend/Device.h"

namespace wind {
class Backend {
public:
    static void Init();
    Backend();

    [[nodiscard]] static auto& GetGPUDevice() { return *s_instance->m_device; }
    [[nodiscard]] static auto  GetBackend() { return s_instance; }

private:
    Backend(const Backend&)                  = delete;
    Backend& operator=(const Backend& other) = delete;

    std::unique_ptr<GPUDevice> m_device;
    
    static std::shared_ptr<Backend> s_instance;
};

}; // namespace wind
