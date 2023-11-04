#pragma once

#include "RenderBackend/Pipeline.h"

namespace wind {
class VertexFactory;
class RasterShader;

class Material {
public:
    enum class ShadingModel : u8 { UnLit = 0, Lit };
    enum class BlendMode : u8 { Opaque, Tanslucency, Additive };

    struct Desc {
        std::string  debugName;
        ShadingModel ShadingModel;
        BlendMode    blendMode;
        RenderState  renderState;
    };

    void SetInput(); // set material parameter

    static Ref<Material> Create(const Desc& desc);
    
private:
    std::string                    m_debugName;
    std::vector<vk::DescriptorSet> m_descriptorSets;
    BlendMode                      m_blendMode;
    ShadingModel                   m_shadingModel;
};
} // namespace wind