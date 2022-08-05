#pragma once

typedef struct lua_State lua_State;

namespace LESSR
{
namespace LuaLib
{
    void openLibs(lua_State* L);
}
}