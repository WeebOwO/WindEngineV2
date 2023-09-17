#include "Engine/EntryPoint.h"

#include "Engine/Window.h"
#include "GLFW/glfw3.h"

#include "Renderer/SceneRenderer.h"

class Editor : public wind::Application {
public:
    Editor() {
        uint32_t width = 1920, height = 1080;
        std::string title = "WindEngineV2";
        m_window = std::make_unique<wind::Window>(width, height, title);
    }

    void Run() override {
                
        wind::Engine engine(m_window.get());       
        engine.Run(); 
    }

private:
    std::unique_ptr<wind::Window> m_window;
};

std::unique_ptr<wind::Application> wind::CreateClientApplication() { 
    return std::make_unique<Editor>(); 
}