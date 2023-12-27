#pragma once

#include "Renderer/MeshPass.h"

namespace wind
{
    struct View
    {
        std::array<MeshDrawCommand, MeshPassType::Count> visiableMeshDrawCommands;
    };
} // namespace wind