export module BackendVK;

import :Device;

namespace wind {
export class Backend {
public:
    static void Init();
private:
    GPUDevice m_device;
};
} // namespace wind
