#pragma once

#include "entt/entt.hpp"

namespace wind {
class Scene;
class Entity {
public:
    Entity() = default;
    Entity(entt::entity handle, Scene* scene) : m_enttHandle(handle), m_scene(scene) {}

private:
    entt::entity m_enttHandle;
    Scene*       m_scene;
};

}; // namespace wind