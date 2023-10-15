#pragma once

#include "RenderBackend/Pipeline.h"
#include "RenderBackend/RasterShader.h"

namespace wind {
class Material {
public:
    enum class ShadingModel { UnLit = 0, Lit };
    enum class BlendMode { Opaque, Tanslucency };

    struct CreateInfo {
        std::string  debugName;
        ShadingModel shaingModel;
    };

    static Scope<Material> Create(const CreateInfo& createInfo);
     
private:
    ShadingModel                  m_shadingModel;
    Scope<RasterShader> m_rasterShader;
    std::string                   m_debugName;
    
};

} // namespace wind