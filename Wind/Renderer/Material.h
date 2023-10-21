#pragma once

#include "RenderBackend/Pipeline.h"
#include "RenderBackend/RasterShader.h"

namespace wind {
class Material {
public:
    enum class ShadingModel { UnLit = 0, Lit };
    enum class BlendMode { Opaque, Tanslucency };

    struct Desc {
        std::string  debugName;
        ShadingModel shaingModel;
    };

    static Ref<Material> Create(const Desc& desc);

private:
    ShadingModel      m_shadingModel;
    Ref<RasterShader> m_rasterShader;
    std::string       m_debugName;
};

} // namespace wind