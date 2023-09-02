#pragma once

#include <memory>

namespace wind {

class Application {
public:
    virtual void Init();
    virtual void Quit();
    virtual void Run() = 0;
};

std::unique_ptr<Application> CreateClientApplication();
} // namespace wind