#include "std.h"

#include <GLFW/glfw3.h>
#include <Imgui/imgui.h>

#include "Engine/Window.h"
#include "Engine/Engine.h"
#include "Engine/EntryPoint.h"

#include "Renderer/Renderer.h"

class Editor : public wind::Application {
public:
    Editor() {
        
    }

    void Run() override {
        wind::Window::WindowDesc desc{
            .width = 1920, .height = 1080, .windowTitle = "WindEngineV2", .vsync = true};

        wind::Scope<wind::Window> window = wind::scope::Create<wind::Window>(desc);

        wind::Engine engine(std::move(window));

        engine.SetImguiCallBack([](wind::Engine& engine) {
            // show demo window
            ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
            ImGui::ShowDemoWindow();
            // show some text
            ImGui::Begin("Vulkan Text");
            ImGui::Text("Hello world");
            ImGui::End();
        });

        engine.Run();
    }
};

wind::Scope<wind::Application> wind::CreateClientApplication() { return scope::Create<Editor>(); }