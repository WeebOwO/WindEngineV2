#pragma once

#include "std.h"

namespace wind {
// manage runtime resource and system
class RuntimeContext {
public:
    void Init();
    void Quit();
};

extern RuntimeContext g_runtimeContext;
} // namespace wind