#pragma once

#include "std.h"

#include "Backend/VulkanHeader.h"

namespace wind {
class VertexFactory;
class RasterShader;

class Material {
public:
    enum class ShadingModel : u8 { UnLit = 0, Lit };
    enum class BlendMode : u8 { Opaque, Tanslucency, Additive };

    Material(const std::string& debugName, ShadingModel shadingModel, BlendMode blendMode,
             RasterShader* rasterShader);

    struct Desc {
        std::string   debugName;
        ShadingModel  ShadingModel;
        BlendMode     blendMode;
        RasterShader* rasterShader;
    };

    void SetInput(); // set material parameter

    static Ref<Material> Create(const Desc& desc);

    auto GetShadingModel() const { return m_shadingModel; }
    auto GetBlendMode() const { return m_blendMode; }
    auto GetShader() const { return m_rasterShader; }

    auto GetMaterialDesc() const noexcept { return m_desc; }

private:
    Desc                           m_desc;
    std::string                    m_debugName;
    RasterShader*                  m_rasterShader;
    BlendMode                      m_blendMode;
    ShadingModel                   m_shadingModel;
    std::vector<vk::DescriptorSet> m_descriptorSets;
};

class MaterialManager {
public:
    void          InitDefaultMaterial();
    Ref<Material> GetMaterial(const std::string& name);

private:
    std::unordered_map<std::string, Ref<Material>> m_materialCaches;
};
} // namespace wind

namespace wind::utils {
    
}