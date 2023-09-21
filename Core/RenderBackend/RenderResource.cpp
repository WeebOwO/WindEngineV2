#include "RenderResource.h"

#include "RenderBackend/Backend.h"

namespace wind {
RenderResource::RenderResource() : device(Backend::GetGPUDevice()) {}
} // namespace wind