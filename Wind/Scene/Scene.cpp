#include "Scene.h"

#include "ECS/Entity.h"

#include "ECS/Component.h"

namespace wind {
GameObject Scene::CreateGameObject(std::string name) {
    auto  gameObject  = GameObject{m_registry.create(), this};
    auto& idComponent = gameObject.AddComponent<IDComponent>();
    gameObject.AddComponent<TagComponent>(name);

    idComponent.id = {};
    m_gameObjectMap[idComponent.id] = gameObject;

    return gameObject;
}
} // namespace wind