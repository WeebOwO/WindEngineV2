#pragma once

#include "Core/Core.h"
#include "RenderBackend/Texture.h"

namespace wind {
struct Texture : public RenderResource {
    
};

class Texture2D final : public Texture {
public:
    NO_COPYABLE(Texture2D);

    void InitRHI() override;
    void RelaseRHI() override;
    
private:
    
};
} // namespace wind