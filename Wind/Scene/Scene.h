#pragma once

#include "std.h"

#include "ECS/Entity.h"
#include "Scene/LightCaster.h"

namespace wind {

struct LightSceneInfo final {
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

class Scene final {
public:
    void Init();

private:
    entt::registry m_Registry;
    LightSceneInfo m_lightSceneInfo;
};
} // namespace wind
