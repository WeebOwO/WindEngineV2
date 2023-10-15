#include "Engine/EntryPoint.h"

#include "Engine/Window.h"
#include "GLFW/glfw3.h"

class Editor : public wind::Application {
public:
    Editor() {}
    void Init() override {}
    void Quit() override {}
    void Run() override {
        uint32_t                  width = 1920, height = 1080;
        std::string               title  = "WindEngineV2";
        wind::Scope<wind::Window> window = wind::scope::Create<wind::Window>(width, height, title);

        wind::Engine engine(std::move(window));
        engine.Run();
    }
};

wind::Scope<wind::Application> wind::CreateClientApplication() { return scope::Create<Editor>(); }