#pragma once

#include "ShaderParameter.h"

#include "RenderBackend/RasterShader.h"

namespace wind {
using namespace vk;

class Material {
public:
    struct CreateInfo {
        std::string  debugName;
        ShadingModel shaingModel;
    };
    
private:
    std::unique_ptr<RasterShader> m_rasterShader;
    std::string                   m_debugName;
};

} // namespace wind