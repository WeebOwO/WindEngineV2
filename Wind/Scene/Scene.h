#pragma once

#include "std.h"

#include "ECS/Entity.h"

namespace wind {
class Scene {
public:
    void Init();
private:
    std::vector<Entity> m_entities;
};
} // namespace wind
