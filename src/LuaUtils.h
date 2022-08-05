#pragma once

#define REGISTER_METHODS_META(__CLASS) \
    luaL_newmetatable(L, #__CLASS); /* create metatable */ \
    luaL_newlib(L, __CLASS##_methods); /* create methods table */ \
    lua_setfield(L, -2, "__index"); /* set methods table as __index */ \
    lua_pop(L, 1) /* pop metatable */ \

#define REGISTER_STATICS(__CLASS) \
    luaL_newlib(L, __CLASS##_statics); /* create static methods table */ \
    lua_setglobal(L, #__CLASS) /* set static methods table in global table */

#define REGISTER_CLASS(__CLASS) \
    REGISTER_METHODS_META(__CLASS); \
    REGISTER_STATICS(__CLASS)

#define REGISTER_CLASS_META(__CLASS) \
    luaL_newmetatable(L, #__CLASS); \
    luaL_newlib(L, __CLASS##_methods); \
    luaL_setfuncs(L, __CLASS##_meta, 1); /* set methods table as upvalue for metamethods */ \
    lua_pop(L, 1); \
    REGISTER_STATICS(__CLASS)

#define REGISTER_ENUM_EX(__NAME, __ARR) \
    lua_createtable(L, 0, sizeof(__ARR)/sizeof(__ARR[0])); \
    for (int i = 0; i < sizeof(__ARR)/sizeof(__ARR[0]); ++i) { \
        lua_pushinteger(L, __ARR[i].value); \
        lua_setfield(L, -2, __ARR[i].name); \
    } \
    lua_setglobal(L, __NAME)

#define REGISTER_ENUM(__ENUM) REGISTER_ENUM_EX(#__ENUM, __ENUM##_enum)

#define CREATE_USERDATA(__TYPE) reinterpret_cast<__TYPE*>(lua_newuserdatauv(L, sizeof(__TYPE), 0))
#define CHECK_USERDATA(L, arg, __TYPE) reinterpret_cast<__TYPE*>(luaL_checkudata(L, arg, #__TYPE))
#define TEST_USERDATA(L, arg, __TYPE) reinterpret_cast<__TYPE*>(luaL_testudata(L, arg, #__TYPE))
#define TO_USERDATA(L, arg, __TYPE) reinterpret_cast<__TYPE*>(lua_touserdata(L, arg))
#define SET_METATABLE(__TYPENAME) luaL_getmetatable(L, __TYPENAME); lua_setmetatable(L, -2)

#define OPERATOR_SAME_CLASS_IMPL(__TYPE, __OP) \
    __TYPE* lhs = CHECK_USERDATA(L, 1, __TYPE); \
    __TYPE* rhs = CHECK_USERDATA(L, 2, __TYPE); \
    __TYPE* res = CREATE_USERDATA(__TYPE); \
    SET_METATABLE(#__TYPE); \
    *res = *lhs __OP *rhs; \
    return 1

#define OPERATOR_CLASS_NUMBER_IMPL(__TYPE, __OP) \
    __TYPE* lhs = CHECK_USERDATA(L, 1, __TYPE); \
    int type = lua_type(L, 2); \
    \
    if (type == LUA_TUSERDATA) \
        luaL_checkudata(L, 2, #__TYPE); \
    else if (type != LUA_TNUMBER) \
        return luaL_argerror(L, 2, "expected " #__TYPE " or number"); \
    \
    __TYPE* res = CREATE_USERDATA(__TYPE); \
    SET_METATABLE(#__TYPE); \
    \
    if (type == LUA_TUSERDATA) { \
        __TYPE* rhs = TO_USERDATA(L, 2, __TYPE); \
        *res = *lhs __OP *rhs; \
    } else if (type == LUA_TNUMBER) { \
        float rhs = lua_tonumber(L, 2); \
        *res = *lhs __OP rhs; \
    } \
    return 1