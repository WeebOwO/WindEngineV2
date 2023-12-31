#pragma once

#include "std.h"

namespace wind {
template <typename T> 
class RenderGraphId;

class RenderGraphHandle {
public:
    using Index = uint16_t;

    RenderGraphHandle() noexcept                                        = default;
    RenderGraphHandle(const RenderGraphHandle& rhs) noexcept            = default;
    RenderGraphHandle& operator=(const RenderGraphHandle& rhs) noexcept = default;

    bool IsInitialized() const noexcept { return m_index != UNINITIALIZED; }
    void Clear() { m_index = UNINITIALIZED; }

    bool operator<(const RenderGraphHandle& rhs) const noexcept { return m_index < rhs.m_index; }

    bool operator==(const RenderGraphHandle& rhs) const noexcept {
        return (m_index == rhs.m_index);
    }

    bool operator!=(const RenderGraphHandle& rhs) const noexcept { return !operator==(rhs); }

    operator bool() const noexcept { return IsInitialized(); }

    explicit RenderGraphHandle(Index index) noexcept : m_index(index) {}

private:
    friend class RenderGraph;
    
    static constexpr uint16_t UNINITIALIZED = std::numeric_limits<Index>::max();
    Index                     m_index       = UNINITIALIZED;
};

template <typename Resource> 
class RenderGraphID : public RenderGraphHandle {
public:
    using RenderGraphHandle::RenderGraphHandle;
    RenderGraphID() noexcept = default;
    explicit RenderGraphID(RenderGraphHandle r) : RenderGraphHandle(r) {}
};
} // namespace wind