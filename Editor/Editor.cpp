#include "Engine/EntryPoint.h"

class Editor : public wind::Application {
public:
    void Run() override {
        wind::Engine engine;
        engine.Run();
    }
};

std::unique_ptr<wind::Application> wind::CreateClientApplication() { 
    return std::make_unique<Editor>(); 
}