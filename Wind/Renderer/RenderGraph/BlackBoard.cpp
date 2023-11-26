#include "BlackBoard.h"

namespace wind {
RenderGraphHandle Blackboard::GetHandle(const std::string& name) const noexcept {
    auto it = m_resources.find(name);
    if(it != m_resources.end()) {
        return it->second;
    }
    return {};
}

void Blackboard::Put(const std::string& name, RenderGraphHandle handle) noexcept {
    operator[](name) = handle;
}

RenderGraphHandle& Blackboard::operator[](const std::string& name) noexcept {
    auto [pos, _] = m_resources.insert_or_assign(name, RenderGraphHandle{});
    return pos->second;
}

void Blackboard::Remove(const std::string& name) noexcept {
    m_resources.erase(name);
}
} // namespace wind