#pragma once

#include "glm/glm.hpp"

namespace wind
{
    struct DirLight
    {
        glm::vec3 direction = {0.0f, 0.0f, 0.0f};
        glm::vec3 radiance  = {0.0f, 0.0f, 0.0f};

        float intensity = 1.0f;
    };

    struct DirectionalLight
    {
        glm::vec3 direction    = {0.0f, 0.0f, 0.0f};
        glm::vec3 radiance     = {0.0f, 0.0f, 0.0f};
        float     intensity    = 0.0f;
        float     shadowAmount = 1.0f;
        bool      castShadows  = true;
    };

    struct PointLight
    {
        glm::vec3 position     = {0.0f, 0.0f, 0.0f};
        float     intensity    = 0.0f;
        glm::vec3 radiance     = {0.0f, 0.0f, 0.0f};
        float     minRadius    = 0.001f;
        float     radius       = 25.0f;
        float     falloff      = 1.f;
        float     sourceSize   = 0.1f;
        bool      castsShadows = true;
        char      padding[3] {0, 0, 0};
    };

    struct SpotLight
    {
        glm::vec3 position         = {0.0f, 0.0f, 0.0f};
        float     intensity        = 0.0f;
        glm::vec3 direction        = {0.0f, 0.0f, 0.0f};
        float     angleAttenuation = 0.0f;
        glm::vec3 radiance         = {0.0f, 0.0f, 0.0f};
        float     range            = 0.1f;
        float     angle            = 0.0f;
        float     falloff          = 1.0f;
        bool      softShadows      = true;
        char      padding0[3] {0, 0, 0};
        bool      castsShadows = true;
        char      padding1[3] {0, 0, 0};
    };
}; // namespace wind