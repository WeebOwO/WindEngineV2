#pragma once

#include "Core/Log.h"
#include "std.h"

#include "Core/UUID.h"

namespace wind {
class RawMesh;

struct Component {
    virtual void Register() {}
    virtual void UnRegister() {}
};

struct IDComponent : public Component {
    UUID id = 0;
};

struct TagComponent : public Component {
    std::string tag;

    TagComponent()                          = default;
    TagComponent(const TagComponent& other) = default;
    TagComponent(const std::string& tag) : tag(tag) {}

    operator std::string&() { return tag; }
    operator const std::string&() const { return tag; }
};

struct MeshComponent : public Component {
    void Register() override;
    void UnRegister() override;

    Ref<RawMesh> meshSource;

    MeshComponent() = default;
};

} // namespace wind