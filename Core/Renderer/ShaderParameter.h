#pragma once

#include "RenderBackend/Texture.h"
#include "RenderBackend/RenderResource.h"

namespace wind {
enum class ShadingModel { UnLit = 0, Lit };

template <ShadingModel Model>
struct ShaderParamer : public RenderResource<RenderResourceType::ShaderParamer> {
      
};

template <>
struct ShaderParamer<ShadingModel::Lit> {

};
};