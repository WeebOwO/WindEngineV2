#include "Scene.h"

#include "ECS/Entity.h"

namespace wind {
    GameObject Scene::CreateGameObject() {
        
        return {m_registry.create(), this};
    }
}