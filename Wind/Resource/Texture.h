#pragma once

#include "Resource/Interface.h"
#include "RenderBackend/Texture.h"

namespace wind {
class Texture2D final : public RenderResourceInterface {
public:
    void InitRHI() override;
    void RelaseRHI() override;

private:
    Ref<GPUTexture2D> m_rhiHandle;
};
} // namespace wind