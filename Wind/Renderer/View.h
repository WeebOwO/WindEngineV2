#pragma once

#include "glm/glm.hpp"

namespace wind {
struct View {
    glm::mat4 view;
    glm::mat4 projection;
};
} // namespace wind