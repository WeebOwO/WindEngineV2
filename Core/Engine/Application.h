#pragma once

#include "std.h"

namespace wind {

class Application {
public:
    virtual void Init() = 0;
    virtual void Quit() = 0;
    virtual void Run()  = 0;
};

Scope<Application> CreateClientApplication();
} // namespace wind