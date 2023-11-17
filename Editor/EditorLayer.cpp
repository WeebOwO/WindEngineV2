#include "EditorLayer.h"

#include "imgui.h"

namespace wind {
void EditorLayer::OnImGuiRender() {
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
    ImGui::ShowDemoWindow();
}
} // namespace wind