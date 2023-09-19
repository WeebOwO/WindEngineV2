#include "Engine/EntryPoint.h"

#include "Engine/Window.h"
#include "GLFW/glfw3.h"

class Editor : public wind::Application {
public:
    Editor() {}

    void Run() override {
        uint32_t                      width = 1920, height = 1080;
        std::string                   title  = "WindEngineV2";
        std::unique_ptr<wind::Window> window = std::make_unique<wind::Window>(width, height, title);

        wind::Engine engine(std::move(window));
        engine.Run();
    }
};

std::unique_ptr<wind::Application> wind::CreateClientApplication() {
    return std::make_unique<Editor>();
}