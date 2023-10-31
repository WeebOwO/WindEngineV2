#include "Component.h"

#include "Resource/Mesh.h"

namespace wind {
void MeshComponent::Register() { meshSource->InitRHI(); }

void MeshComponent::UnRegister() { meshSource->RelaseRHI(); }
}; // namespace wind