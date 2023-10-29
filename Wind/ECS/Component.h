#pragma once

#include "std.h"

namespace wind {
class MeshDesc;

struct Component {};

struct MeshComponent : public Component {
    Ref<MeshDesc> meshDesc;
};

} // namespace wind