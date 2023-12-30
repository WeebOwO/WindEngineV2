#include "Script.h"

#include "Log.h"
#include "std.h"

namespace wind::lua
{
    bool CheckLua(lua_State* state, int r)
    {
        if (r != LUA_OK)
        {
            std::string errorMsg = lua_tostring(state, -1);
            WIND_CORE_ERROR("Lua: {}", errorMsg);
            return false;
        }
        return true;
    }
} // namespace wind::lua