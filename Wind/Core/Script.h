#pragma once

extern "C"
{
#include "lauxlib.h"
#include "lua.h"
#include "lualib.h"
}

namespace wind::lua {
    bool CheckLua(lua_State* state, int r);
}