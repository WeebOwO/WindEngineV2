#pragma once

#include "RenderBackend/Texture.h"

namespace wind {

struct Texture : public RenderResource {
    
};

class Texture2D final : public Texture {
public:
    void InitRHI() override;
    void RelaseRHI() override;
    
private:
    Ref<GPUTexture2D> m_rhiHandle;
};
} // namespace wind