#include "Engine/EntryPoint.h"

#include "Engine/Window.h"
#include "GLFW/glfw3.h"

class Editor : public wind::Application {
public:
    Editor() {}
    void Init() override {}
    void Quit() override {}
    void Run() override {
        wind::Window::WindowDesc desc {
            .width = 1920,
            .height = 1080,
            .windowTitle = "WindEngineV2",
            .vsync = true
        };

        wind::Scope<wind::Window> window = wind::scope::Create<wind::Window>(desc);

        wind::Engine engine(std::move(window));
        engine.Run();
    }
};

wind::Scope<wind::Application> wind::CreateClientApplication() { return scope::Create<Editor>(); }