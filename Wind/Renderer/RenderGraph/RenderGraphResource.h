#pragma once

#include "std.h"

#include "Core/Core.h"
#include "RenderBackend/Texture.h"
#include <unordered_map>

namespace wind {
enum RenderGraphResourceType : u8 { Buffer, Texture };

struct RenderGraphResource {
    RenderGraphResourceType         type;
    std::string                     resoureName;
    std::unordered_set<std::string> producer;
    std::unordered_set<std::string> consumer;
};

struct AttachmentInfo {
    u32        width;
    u32        height;
    vk::Format format{vk::Format::eUndefined};
    u32        samples{1};
    u32        levels{1};
    u32        layers{1};
    bool       persistent{true};
};

class RenderGraphTexture : public RenderGraphResource {
public:
    RenderGraphTexture() = default;
    RenderGraphTexture(const GPUTexture::Desc& desc);

private:
    Ref<GPUTexture> texture;
};

class Blackboard final {
public:
    Blackboard() noexcept  = default;
    ~Blackboard() noexcept = default;

    void Put(const std::string& string, Scope<RenderGraphResource> resourceHandle);

    template <typename T> T* Get(const std::string& resourceName) noexcept {
        return static_cast<T*>(m_resources[resourceName].get());
    }

    void Remove(const std::string& name) noexcept { m_resources.erase(name); }

    void Clear();

private:
    std::unordered_map<std::string, Scope<RenderGraphResource>> m_resources;
};
} // namespace wind