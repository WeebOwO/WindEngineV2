#pragma once

namespace wind {
class Engine {
public:
    Engine();
    ~Engine();
    void Init();
    void Quit();
    
    void Run();
private:
    void RenderTick();
    void LogicTick();
};

} // namespace wind