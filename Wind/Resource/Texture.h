#pragma once

#include "Core/Core.h"
#include "Backend/Texture.h"

namespace wind {
struct Texture : public RenderResource {
    
};

class Texture2D final : public Texture {
public:
    NO_COPYABLE(Texture2D);

    void InitRHI() override;
    void ReleaseRHI() override;
    
private:
    
};
} // namespace wind