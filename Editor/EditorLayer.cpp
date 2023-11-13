#include "EditorLayer.h"

#include "imgui.h"

namespace wind {
void EditorLayer::OnImGuiRender() {
    ImGui::ShowDemoWindow();
}
} // namespace wind