#pragma once

#include "Engine/Engine.h"
#include "Engine/Application.h"

extern std::unique_ptr<wind::Application> CreateClientApplication();

namespace wind {
    int GuardedMain(int argc, char **argv) {
        auto app = CreateClientApplication();
        app->Run();
        return 0;
    }
}

int main(int argc, char **argv) {
    return wind::GuardedMain(argc, argv);
}

