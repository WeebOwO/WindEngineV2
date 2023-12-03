#pragma once

#include "std.h"

#include "RenderGraphID.h"

namespace wind {
class Blackboard final {
public:
    Blackboard() noexcept  = default;
    ~Blackboard() noexcept = default;

    void Put(const std::string& string, RenderGraphHandle handle) noexcept;
    void Remove(const std::string& name) noexcept;

    RenderGraphHandle& operator[](const std::string& name) noexcept;

    template <typename T> 
    RenderGraphID<T>* Get(const std::string& resourceName) noexcept {
        return static_cast<RenderGraphID<T>>(GetHandle(resourceName));
    }   
    
private:
    RenderGraphHandle GetHandle(const std::string& string) const noexcept;

    std::unordered_map<std::string, RenderGraphHandle> m_resources;
};
} // namespace wind