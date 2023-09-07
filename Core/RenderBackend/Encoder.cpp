#include "Encoder.h"

#include "RenderBackend/Backend.h"

namespace wind {
CommandEncoder::~CommandEncoder() {
    auto& device = Backend::GetGPUDevice();
    device.DestroyCommandEncoder(*this);
}
} // namespace wind