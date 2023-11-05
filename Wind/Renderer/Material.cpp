#include "Material.h"

namespace wind {
Material::Material(const std::string& debugName, ShadingModel shadingModel, BlendMode blendMode)
    : m_debugName(debugName), m_shadingModel(shadingModel), m_blendMode(blendMode) {
    // we must init shader map and then init all material
}

Ref<Material> Material::Create(const Desc& desc) {
    return ref::Create<Material>(desc.debugName, desc.ShadingModel, desc.blendMode);
}

void MaterialManager::InitDefaultMaterial() {
    // default lit
    Material::Desc desc{
        .debugName    = "default_lit",
        .ShadingModel = Material::ShadingModel::Lit,
        .blendMode    = Material::BlendMode::Opaque,
    };
    m_materialCaches[desc.debugName] = Material::Create(desc);

    desc = {
        .debugName    = "default_unlit",
        .ShadingModel = Material::ShadingModel::UnLit,
        .blendMode    = Material::BlendMode::Opaque,
    };

    m_materialCaches[desc.debugName] = Material::Create(desc);
}
} // namespace wind