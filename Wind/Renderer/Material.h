#pragma once

#include "RenderBackend/Pipeline.h"

namespace wind {
class VertexFactory;
class RasterShader;

class Material {
public:
    enum class ShadingModel { UnLit = 0, Lit };
    enum class BlendMode { Opaque, Tanslucency, Additive};

    static Ref<Material> Create(ShadingModel ShadingModel, BlendMode blendMode);

private:
    std::string       m_debugName;
    RasterState       m_rasterState;
    ShadingModel      m_shadingModel;
};

} // namespace wind