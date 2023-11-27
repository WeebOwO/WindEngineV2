#include "ResourceRegistry.h"

#include "RenderGraph.h"

namespace wind {
vk::RenderingInfo ResourceRegistry::GetRenderingInfo() const noexcept {
    return static_cast<RenderPassNode*>(m_passNode)->GetRenderingInfo();
}
} // namespace wind