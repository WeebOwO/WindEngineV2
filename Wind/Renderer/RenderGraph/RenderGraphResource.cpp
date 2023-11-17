#include "RenderGraphResource.h"

namespace wind {

void Blackboard::Put(const std::string& string, Scope<RenderGraphResource> resourceHandle) {
    m_resources[string] = std::move(resourceHandle);
}

void Blackboard::Clear() { m_resources.clear(); }
} // namespace wind