#include "Backend.h"

#include "std.h"

namespace wind {
Scope<Backend> Backend::s_instance = nullptr;

Backend::Backend() { m_device = scope::Create<GPUDevice>(); }

void Backend::Init() { s_instance = scope::Create<Backend>(); }
} // namespace wind