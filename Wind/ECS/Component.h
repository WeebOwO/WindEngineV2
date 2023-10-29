#pragma once

#include "std.h"

#include "Core/UUID.h"

namespace wind {
class MeshDesc;

struct IDComponent {
    UUID id = 0;
};

struct TagComponent {
    std::string tag;

    TagComponent()                          = default;
    TagComponent(const TagComponent& other) = default;
    TagComponent(const std::string& tag) : tag(tag) {}

    operator std::string&() { return tag; }
    operator const std::string&() const { return tag; }
};

struct MeshComponent {
    Ref<MeshDesc> meshDesc;
};

} // namespace wind