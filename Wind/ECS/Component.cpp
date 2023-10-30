#include "Component.h"

#include "Resource/Mesh.h"

namespace wind {
void MeshComponent::RegisterComponent() { meshSource->InitRHI(); }

void MeshComponent::UnRegister() { meshSource->RelaseRHI(); }
}; // namespace wind