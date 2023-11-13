#include "Material.h"

#include "Engine/RuntimeContext.h"

namespace wind {
Material::Material(const std::string& debugName, ShadingModel shadingModel, BlendMode blendMode,
                   RasterShader* rasterShader)
    : m_debugName(debugName), m_shadingModel(shadingModel), m_blendMode(blendMode),
      m_rasterShader(rasterShader) {
    // we must init shader map and then init all material
}

Ref<Material> Material::Create(const Desc& desc) {
    return ref::Create<Material>(desc.debugName, desc.ShadingModel, desc.blendMode,
                                 desc.rasterShader);
}

void MaterialManager::InitDefaultMaterial() {
    // default lit
    auto shaderMap = g_runtimeContext.shaderMap.get();

    Material::Desc desc{.debugName    = "default_lit",
                        .ShadingModel = Material::ShadingModel::Lit,
                        .blendMode    = Material::BlendMode::Opaque,
                        .rasterShader = shaderMap->GetRasterShader("BasePassShader").get()};
    
    m_materialCaches[desc.debugName] = Material::Create(desc);

    desc = {.debugName    = "default_unlit",
            .ShadingModel = Material::ShadingModel::UnLit,
            .blendMode    = Material::BlendMode::Opaque,
            .rasterShader = shaderMap->GetRasterShader("BasePassShader").get()};

    m_materialCaches[desc.debugName] = Material::Create(desc);
}

Ref<Material> MaterialManager::GetMaterial(const std::string& name) {
    assert(m_materialCaches.contains(name));
    return m_materialCaches[name];
}
} // namespace wind