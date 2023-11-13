#pragma once

#include "Engine/Layer.h"

namespace wind {
class EditorLayer : public Layer {
public:
    EditorLayer() : Layer("EditorLayer") {}
    ~EditorLayer() = default;
    
    void OnImGuiRender() override;
};
} // namespace wind