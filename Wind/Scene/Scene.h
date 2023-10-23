#pragma once

#include "std.h"

#include "ECS/Entity.h"
#include "Scene/LightCaster.h"

namespace wind {

struct LightSceneInfo {
    static constexpr size_t MaxDirectionalLights = 4;
    DirectionalLight        DirectionalLights[MaxDirectionalLights];
    std::vector<PointLight> PointLights;
    std::vector<SpotLight>  SpotLights;
    [[nodiscard]] auto      GetPointLightsSize() const {
        return (uint32_t)(PointLights.size() * sizeof(PointLight));
    }
    [[nodiscard]] auto GetSpotLightsSize() const {
        return (uint32_t)(SpotLights.size() * sizeof(SpotLight));
    }
};

class Scene {
public:
    void Init();

private:
    std::vector<Entity> m_entities;
};
} // namespace wind
