#pragma once

#include "Engine/Layer.h"

namespace wind {
class EditorLayer : public Layer {
public:
    void OnImGuiRender() override;
};
} // namespace wind