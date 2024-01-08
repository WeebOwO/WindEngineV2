#pragma once

#include "Renderer/MeshPass.h"

namespace wind
{
    class ViewUniformBuffer
    {
        
    };
    
    struct View
    {
        using ViewUniformBufferRef = Ref<ViewUniformBuffer>;

        ViewUniformBufferRef                             viewUniformBuffer;
        std::array<MeshDrawCommand, MeshPassType::Count> visiableMeshDrawCommands;
    };
} // namespace wind