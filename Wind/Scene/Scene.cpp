#include "Scene.h"

#include "ECS/Component.h"
#include "ECS/Entity.h"

namespace wind {
GameObject Scene::CreateGameObject(std::string name) {
    auto  gameObject  = GameObject{m_registry.create(), this};
    auto& idComponent = gameObject.AddComponent<IDComponent>();
    gameObject.AddComponent<TagComponent>(name);

    idComponent.id                  = {};
    m_gameObjectMap[idComponent.id] = gameObject;
    m_isDirty                       = true;
    return gameObject;
}

void Scene::Update() {
    if(!m_isDirty) return;
    // clear up mesh pass 
    
}
} // namespace wind