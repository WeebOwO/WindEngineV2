#pragma once

#include "std.h"

#include <entt/entt.hpp>

#include "Scene/LightCaster.h"

namespace wind {
class GameObject;

struct LightSceneInfo final {
    static constexpr size_t MaxDirectionalLights = 4;
    DirectionalLight        DirectionalLights[MaxDirectionalLights];
    std::vector<PointLight> PointLights;
    std::vector<SpotLight>  SpotLights;

    [[nodiscard]] auto GetPointLightsSize() const {
        return (uint32_t)(PointLights.size() * sizeof(PointLight));
    }
    [[nodiscard]] auto GetSpotLightsSize() const {
        return (uint32_t)(SpotLights.size() * sizeof(SpotLight));
    }
};

class Scene final {
public:
    void Init();

    GameObject CreateGameObject();
    
private:
    friend class GameObject;
    entt::registry m_registry;
    LightSceneInfo m_lightSceneInfo;
};
} // namespace wind
