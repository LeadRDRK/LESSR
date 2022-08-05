#include <LESSR/LuaLib.h>
#include <LESSR/Vector2.h>
#include <LESSR/Vector3.h>
#include <LESSR/Vector4.h>
#include <LESSR/Color3.h>
#include <LESSR/Color4.h>
#include <LESSR/ImageData.h>
#include <LESSR/Enums.h>
#include <lua.hpp>
#include <cstring>
#include "LuaUtils.h"

namespace LESSR
{
namespace LuaLib
{

struct Enum
{
    const char* name;
    int value;
};

/***********  Vector2  ***********/

static int Vector2_new(lua_State* L)
{
    Vector2* self = CREATE_USERDATA(Vector2);
    SET_METATABLE("Vector2");

    if (lua_gettop(L) > 1) /* has something other than the ud we just pushed */
    {
        self->x = luaL_checknumber(L, 1);
        self->y = luaL_checknumber(L, 2);
    }
    return 1;
}

static int Vector2_set(lua_State* L)
{
    Vector2* self = CHECK_USERDATA(L, 1, Vector2);
    self->x = luaL_checknumber(L, 2);
    self->y = luaL_checknumber(L, 3);
    return 0;
}

static int Vector2_zero(lua_State* L)
{
    Vector2* self = CHECK_USERDATA(L, 1, Vector2);
    self->zero();
    return 0;
}

static int Vector2_lerp(lua_State* L)
{
    Vector2* self = CHECK_USERDATA(L, 1, Vector2);
    Vector2* goal = CHECK_USERDATA(L, 2, Vector2);
    float alpha = luaL_checknumber(L, 3);

    Vector2* res = CREATE_USERDATA(Vector2);
    SET_METATABLE("Vector2");

    self->lerp(*goal, alpha, res);
    return 1;
}

static int Vector2_dot(lua_State* L)
{
    Vector2* self = CHECK_USERDATA(L, 1, Vector2);
    Vector2* other = CHECK_USERDATA(L, 2, Vector2);

    lua_pushnumber(L, self->dot(*other));
    return 1;
}

static int Vector2_distance(lua_State* L)
{
    Vector2* self = CHECK_USERDATA(L, 1, Vector2);
    Vector2* other = CHECK_USERDATA(L, 2, Vector2);

    lua_pushnumber(L, self->distance(*other));
    return 1;
}

static int Vector2_length(lua_State* L)
{
    Vector2* self = CHECK_USERDATA(L, 1, Vector2);
    lua_pushnumber(L, self->length());
    return 1;
}

static int Vector2__index(lua_State* L)
{
    Vector2* self = CHECK_USERDATA(L, 1, Vector2);
    const char* k = luaL_checkstring(L, 2);
    
    if (strcmp(k, "x") == 0)
        lua_pushnumber(L, self->x);
    else if (strcmp(k, "y") == 0)
        lua_pushnumber(L, self->y);
    else /* get from methods table */
        lua_getfield(L, lua_upvalueindex(1), k);
    
    return 1;
}

static int Vector2__newindex(lua_State* L)
{
    Vector2* self = CHECK_USERDATA(L, 1, Vector2);
    const char* k = luaL_checkstring(L, 2);
    float v = luaL_checknumber(L, 3);

    if (strcmp(k, "x") == 0)
        self->x = v;
    else if (strcmp(k, "y") == 0)
        self->y = v;
    else
        luaL_error(L, "invalid property \"%s\"", k);
    
    return 0;
}

static int Vector2__add(lua_State* L)
{
    OPERATOR_SAME_CLASS_IMPL(Vector2, +);
}

static int Vector2__sub(lua_State* L)
{
    OPERATOR_SAME_CLASS_IMPL(Vector2, -);
}

static int Vector2__mul(lua_State* L)
{
    OPERATOR_CLASS_NUMBER_IMPL(Vector2, *);
}

static int Vector2__div(lua_State* L)
{
    OPERATOR_CLASS_NUMBER_IMPL(Vector2, /);
}

static const luaL_Reg Vector2_statics[] = {
    {"new", Vector2_new},
    {NULL, NULL}
};

static const luaL_Reg Vector2_methods[] = {
    {"set", Vector2_set},
    {"zero", Vector2_zero},
    {"lerp", Vector2_lerp},
    {"dot", Vector2_dot},
    {"distance", Vector2_distance},
    {"length", Vector2_length},
    {NULL, NULL}
};

static const luaL_Reg Vector2_meta[] = {
    {"__index", Vector2__index},
    {"__newindex", Vector2__newindex},
    {"__add", Vector2__add},
    {"__sub", Vector2__sub},
    {"__mul", Vector2__mul},
    {"__div", Vector2__div},
    {NULL, NULL}
};

/***********  Vector3  ***********/

static int Vector3_new(lua_State* L)
{
    Vector3* self = CREATE_USERDATA(Vector3);
    SET_METATABLE("Vector3");

    if (lua_gettop(L) > 1)
    {
        int type = lua_type(L, 1);
        if (type == LUA_TUSERDATA)
        {
            Vector2* v = CHECK_USERDATA(L, 1, Vector2);
            *self = *v;
        }
        else
        {
            self->x = luaL_checknumber(L, 1);
            self->y = luaL_checknumber(L, 2);
            self->z = luaL_checknumber(L, 3);
        }
    }
    return 1;
}

static int Vector3_set(lua_State* L)
{
    Vector3* self = CHECK_USERDATA(L, 1, Vector3);
    self->x = luaL_checknumber(L, 2);
    self->y = luaL_checknumber(L, 3);
    self->z = luaL_checknumber(L, 4);
    return 0;
}

static int Vector3_zero(lua_State* L)
{
    Vector3* self = CHECK_USERDATA(L, 1, Vector3);
    self->zero();
    return 0;
}

static int Vector3_lerp(lua_State* L)
{
    Vector3* self = CHECK_USERDATA(L, 1, Vector3);
    Vector3* goal = CHECK_USERDATA(L, 2, Vector3);
    float alpha = luaL_checknumber(L, 3);

    Vector3* res = CREATE_USERDATA(Vector3);
    SET_METATABLE("Vector3");

    self->lerp(*goal, alpha, res);
    return 1;
}

static int Vector3_cross(lua_State* L)
{
    Vector3* self = CHECK_USERDATA(L, 1, Vector3);
    Vector3* other = CHECK_USERDATA(L, 2, Vector3);

    Vector3* res = CREATE_USERDATA(Vector3);
    SET_METATABLE("Vector3");

    self->cross(*other, res);
    return 1;
}

static int Vector3_dot(lua_State* L)
{
    Vector3* self = CHECK_USERDATA(L, 1, Vector3);
    Vector3* other = CHECK_USERDATA(L, 2, Vector3);

    lua_pushnumber(L, self->dot(*other));
    return 1;
}

static int Vector3_distance(lua_State* L)
{
    Vector3* self = CHECK_USERDATA(L, 1, Vector3);
    Vector3* other = CHECK_USERDATA(L, 2, Vector3);

    lua_pushnumber(L, self->distance(*other));
    return 1;
}

static int Vector3_length(lua_State* L)
{
    Vector3* self = CHECK_USERDATA(L, 1, Vector3);
    lua_pushnumber(L, self->length());
    return 1;
}

static int Vector3_normalize(lua_State* L)
{
    Vector3* self = CHECK_USERDATA(L, 1, Vector3);
    self->normalize();
    return 0;
}

static int Vector3__index(lua_State* L)
{
    Vector3* self = CHECK_USERDATA(L, 1, Vector3);
    const char* k = luaL_checkstring(L, 2);
    
    if (strcmp(k, "x") == 0)
        lua_pushnumber(L, self->x);
    else if (strcmp(k, "y") == 0)
        lua_pushnumber(L, self->y);
    else if (strcmp(k, "z") == 0)
        lua_pushnumber(L, self->z);
    else /* get from methods table */
        lua_getfield(L, lua_upvalueindex(1), k);
    
    return 1;
}

static int Vector3__newindex(lua_State* L)
{
    Vector3* self = CHECK_USERDATA(L, 1, Vector3);
    const char* k = luaL_checkstring(L, 2);
    float v = luaL_checknumber(L, 3);

    if (strcmp(k, "x") == 0)
        self->x = v;
    else if (strcmp(k, "y") == 0)
        self->y = v;
    else if (strcmp(k, "z") == 0)
        self->z = v;
    else
        luaL_error(L, "invalid property \"%s\"", k);
    
    return 0;
}

static int Vector3__add(lua_State* L)
{
    OPERATOR_SAME_CLASS_IMPL(Vector3, +);
}

static int Vector3__sub(lua_State* L)
{
    OPERATOR_SAME_CLASS_IMPL(Vector3, -);
}

static int Vector3__mul(lua_State* L)
{
    OPERATOR_CLASS_NUMBER_IMPL(Vector3, *);
}

static int Vector3__div(lua_State* L)
{
    OPERATOR_CLASS_NUMBER_IMPL(Vector3, /);
}

static const luaL_Reg Vector3_statics[] = {
    {"new", Vector3_new},
    {NULL, NULL}
};

static const luaL_Reg Vector3_methods[] = {
    {"set", Vector3_set},
    {"zero", Vector3_zero},
    {"lerp", Vector3_lerp},
    {"dot", Vector3_dot},
    {"cross", Vector3_cross},
    {"distance", Vector3_distance},
    {"length", Vector3_length},
    {"normalize", Vector3_normalize},
    {NULL, NULL}
};

static const luaL_Reg Vector3_meta[] = {
    {"__index", Vector3__index},
    {"__newindex", Vector3__newindex},
    {"__add", Vector3__add},
    {"__sub", Vector3__sub},
    {"__mul", Vector3__mul},
    {"__div", Vector3__div},
    {NULL, NULL}
};

/***********  Vector4  ***********/

static int Vector4_new(lua_State* L)
{
    Vector4* self = CREATE_USERDATA(Vector4);
    SET_METATABLE("Vector4");

    if (lua_gettop(L) > 1)
    {
        int type = lua_type(L, 1);
        if (type == LUA_TUSERDATA)
        {
            if (luaL_testudata(L, 1, "Vector3"))
            {
                Vector3* v = TO_USERDATA(L, 1, Vector3);
                *self = *v;
            }
            else if (luaL_testudata(L, 1, "Vector2"))
            {
                Vector2* v = TO_USERDATA(L, 1, Vector2);
                *self = *v;
            }
            else
                luaL_argerror(L, 1, "expected Vector3 or Vector2");
        }
        else
        {
            self->x = luaL_checknumber(L, 1);
            self->y = luaL_checknumber(L, 2);
            self->z = luaL_checknumber(L, 3);
            self->w = luaL_checknumber(L, 4);
        }
    }
    return 1;
}

static int Vector4_set(lua_State* L)
{
    Vector4* self = CHECK_USERDATA(L, 1, Vector4);
    self->x = luaL_checknumber(L, 2);
    self->y = luaL_checknumber(L, 3);
    self->z = luaL_checknumber(L, 4);
    self->w = luaL_checknumber(L, 5);
    return 0;
}

static int Vector4_zero(lua_State* L)
{
    Vector4* self = CHECK_USERDATA(L, 1, Vector4);
    self->zero();
    return 0;
}

static int Vector4_lerp(lua_State* L)
{
    Vector4* self = CHECK_USERDATA(L, 1, Vector4);
    Vector4* goal = CHECK_USERDATA(L, 2, Vector4);
    float alpha = luaL_checknumber(L, 3);

    Vector4* res = CREATE_USERDATA(Vector4);
    SET_METATABLE("Vector4");

    self->lerp(*goal, alpha, res);
    return 1;
}

static int Vector4_dot(lua_State* L)
{
    Vector4* self = CHECK_USERDATA(L, 1, Vector4);
    Vector4* other = CHECK_USERDATA(L, 2, Vector4);

    lua_pushnumber(L, self->dot(*other));
    return 1;
}

static int Vector4_distance(lua_State* L)
{
    Vector4* self = CHECK_USERDATA(L, 1, Vector4);
    Vector4* other = CHECK_USERDATA(L, 2, Vector4);

    lua_pushnumber(L, self->distance(*other));
    return 1;
}

static int Vector4_length(lua_State* L)
{
    Vector4* self = CHECK_USERDATA(L, 1, Vector4);
    lua_pushnumber(L, self->length());
    return 1;
}

static int Vector4_normalize(lua_State* L)
{
    Vector4* self = CHECK_USERDATA(L, 1, Vector4);
    self->normalize();
    return 0;
}

static int Vector4__index(lua_State* L)
{
    Vector4* self = CHECK_USERDATA(L, 1, Vector4);
    const char* k = luaL_checkstring(L, 2);
    
    if (strcmp(k, "x") == 0)
        lua_pushnumber(L, self->x);
    else if (strcmp(k, "y") == 0)
        lua_pushnumber(L, self->y);
    else if (strcmp(k, "z") == 0)
        lua_pushnumber(L, self->z);
    else if (strcmp(k, "w") == 0)
        lua_pushnumber(L, self->w);
    else /* get from methods table */
        lua_getfield(L, lua_upvalueindex(1), k);
    
    return 1;
}

static int Vector4__newindex(lua_State* L)
{
    Vector4* self = CHECK_USERDATA(L, 1, Vector4);
    const char* k = luaL_checkstring(L, 2);
    float v = luaL_checknumber(L, 3);

    if (strcmp(k, "x") == 0)
        self->x = v;
    else if (strcmp(k, "y") == 0)
        self->y = v;
    else if (strcmp(k, "z") == 0)
        self->z = v;
    else if (strcmp(k, "w") == 0)
        self->w = v;
    else
        luaL_error(L, "invalid property \"%s\"", k);
    
    return 0;
}

static int Vector4__add(lua_State* L)
{
    OPERATOR_SAME_CLASS_IMPL(Vector4, +);
}

static int Vector4__sub(lua_State* L)
{
    OPERATOR_SAME_CLASS_IMPL(Vector4, -);
}

static int Vector4__mul(lua_State* L)
{
    OPERATOR_CLASS_NUMBER_IMPL(Vector4, *);
}

static int Vector4__div(lua_State* L)
{
    OPERATOR_CLASS_NUMBER_IMPL(Vector4, /);
}

static const luaL_Reg Vector4_statics[] = {
    {"new", Vector4_new},
    {NULL, NULL}
};

static const luaL_Reg Vector4_methods[] = {
    {"set", Vector4_set},
    {"zero", Vector4_zero},
    {"lerp", Vector4_lerp},
    {"dot", Vector4_dot},
    {"distance", Vector4_distance},
    {"length", Vector4_length},
    {"normalize", Vector4_normalize},
    {NULL, NULL}
};

static const luaL_Reg Vector4_meta[] = {
    {"__index", Vector4__index},
    {"__newindex", Vector4__newindex},
    {"__add", Vector4__add},
    {"__sub", Vector4__sub},
    {"__mul", Vector4__mul},
    {"__div", Vector4__div},
    {NULL, NULL}
};

/***********  Color3  ***********/

static int Color3_new(lua_State* L)
{
    Color3* self = CREATE_USERDATA(Color3);
    SET_METATABLE("Color3");

    if (lua_gettop(L) > 1)
    {
        int type = lua_type(L, 1);
        if (type == LUA_TUSERDATA)
        {
            Color4* v = CHECK_USERDATA(L, 1, Color4);
            *self = *v;
        }
        else /* any other type errors will show up as "expected number" */
        {
            self->r = luaL_checkinteger(L, 1);
            self->g = luaL_checkinteger(L, 2);
            self->b = luaL_checkinteger(L, 3);
        }
    }
    return 1;
}

static int Color3_set(lua_State* L)
{
    Color3* self = CHECK_USERDATA(L, 1, Color3);
    self->r = luaL_checkinteger(L, 2);
    self->g = luaL_checkinteger(L, 3);
    self->b = luaL_checkinteger(L, 4);
    return 0;
}

static int Color3_lerp(lua_State* L)
{
    Color3* self = CHECK_USERDATA(L, 1, Color3);
    Color3* goal = CHECK_USERDATA(L, 2, Color3);
    float alpha = luaL_checknumber(L, 3);

    Color3* res = CREATE_USERDATA(Color3);
    SET_METATABLE("Color3");

    self->lerp(*goal, alpha, res);
    return 1;
}

static int Color3__index(lua_State* L)
{
    Color3* self = CHECK_USERDATA(L, 1, Color3);
    const char* k = luaL_checkstring(L, 2);
    
    if (strcmp(k, "r") == 0)
        lua_pushinteger(L, self->r);
    else if (strcmp(k, "g") == 0)
        lua_pushinteger(L, self->g);
    else if (strcmp(k, "b") == 0)
        lua_pushinteger(L, self->b);
    else /* get from methods table */
        lua_getfield(L, lua_upvalueindex(1), k);
    
    return 1;
}

static int Color3__newindex(lua_State* L)
{
    Color3* self = CHECK_USERDATA(L, 1, Color3);
    const char* k = luaL_checkstring(L, 2);
    uint8_t v = luaL_checkinteger(L, 3);

    if (strcmp(k, "r") == 0)
        self->r = v;
    else if (strcmp(k, "g") == 0)
        self->g = v;
    else if (strcmp(k, "b") == 0)
        self->b = v;
    else
        luaL_error(L, "invalid property \"%s\"", k);
    
    return 0;
}

static int Color3__add(lua_State* L)
{
    OPERATOR_SAME_CLASS_IMPL(Color3, +);
}

static int Color3__sub(lua_State* L)
{
    OPERATOR_SAME_CLASS_IMPL(Color3, -);
}

static int Color3__mul(lua_State* L)
{
    OPERATOR_CLASS_NUMBER_IMPL(Color3, *);
}

static const luaL_Reg Color3_statics[] = {
    {"new", Color3_new},
    {NULL, NULL}
};

static const luaL_Reg Color3_methods[] = {
    {"set", Color3_set},
    {"lerp", Color3_lerp},
    {NULL, NULL}
};

static const luaL_Reg Color3_meta[] = {
    {"__index", Color3__index},
    {"__newindex", Color3__newindex},
    {"__add", Color3__add},
    {"__sub", Color3__sub},
    {"__mul", Color3__mul},
    {NULL, NULL}
};

/***********  Color4  ***********/

static int Color4_new(lua_State* L)
{
    Color4* self = CREATE_USERDATA(Color4);
    SET_METATABLE("Color4");

    if (lua_gettop(L) > 1)
    {
        int type = lua_type(L, 1);
        if (type == LUA_TUSERDATA)
        {
            Color3* v = CHECK_USERDATA(L, 1, Color3);
            *self = *v;
        }
        else
        {
            self->r = luaL_checkinteger(L, 1);
            self->g = luaL_checkinteger(L, 2);
            self->b = luaL_checkinteger(L, 3);
            self->a = luaL_checkinteger(L, 4);
        }
    }
    return 1;
}

static int Color4_set(lua_State* L)
{
    Color4* self = CHECK_USERDATA(L, 1, Color4);
    self->r = luaL_checkinteger(L, 2);
    self->g = luaL_checkinteger(L, 3);
    self->b = luaL_checkinteger(L, 4);
    self->a = luaL_checkinteger(L, 5);
    return 0;
}

static int Color4_lerp(lua_State* L)
{
    Color4* self = CHECK_USERDATA(L, 1, Color4);
    Color4* goal = CHECK_USERDATA(L, 2, Color4);
    float alpha = luaL_checknumber(L, 3);

    Color4* res = CREATE_USERDATA(Color4);
    SET_METATABLE("Color4");

    self->lerp(*goal, alpha, res);
    return 1;
}

static int Color4__index(lua_State* L)
{
    Color4* self = CHECK_USERDATA(L, 1, Color4);
    const char* k = luaL_checkstring(L, 2);
    
    if (strcmp(k, "r") == 0)
        lua_pushinteger(L, self->r);
    else if (strcmp(k, "g") == 0)
        lua_pushinteger(L, self->g);
    else if (strcmp(k, "b") == 0)
        lua_pushinteger(L, self->b);
    else if (strcmp(k, "a") == 0)
        lua_pushinteger(L, self->a);
    else /* get from methods table */
        lua_getfield(L, lua_upvalueindex(1), k);
    
    return 1;
}

static int Color4__newindex(lua_State* L)
{
    Color4* self = CHECK_USERDATA(L, 1, Color4);
    const char* k = luaL_checkstring(L, 2);
    uint8_t v = luaL_checkinteger(L, 3);

    if (strcmp(k, "r") == 0)
        self->r = v;
    else if (strcmp(k, "g") == 0)
        self->g = v;
    else if (strcmp(k, "b") == 0)
        self->b = v;
    else if (strcmp(k, "a") == 0)
        self->a = v;
    else
        luaL_error(L, "invalid property \"%s\"", k);
    
    return 0;
}

static int Color4__add(lua_State* L)
{
    OPERATOR_SAME_CLASS_IMPL(Color4, +);
}

static int Color4__sub(lua_State* L)
{
    OPERATOR_SAME_CLASS_IMPL(Color4, -);
}

static int Color4__mul(lua_State* L)
{
    OPERATOR_CLASS_NUMBER_IMPL(Color4, *);
}

static const luaL_Reg Color4_statics[] = {
    {"new", Color4_new},
    {NULL, NULL}
};

static const luaL_Reg Color4_methods[] = {
    {"set", Color4_set},
    {"lerp", Color4_lerp},
    {NULL, NULL}
};

static const luaL_Reg Color4_meta[] = {
    {"__index", Color4__index},
    {"__newindex", Color4__newindex},
    {"__add", Color4__add},
    {"__sub", Color4__sub},
    {"__mul", Color4__mul},
    {NULL, NULL}
};

/***********  Matrix4  ***********/

static int Matrix4_set_i(lua_State* L, int ud, int arr);

static int Matrix4_new(lua_State* L)
{
    Matrix4* self = CREATE_USERDATA(Matrix4);
    SET_METATABLE("Matrix4");

    Matrix4_set_i(L, -1, 1);
    return 1;
}

static int Matrix4_createLookAt(lua_State* L)
{
    Matrix4* self = CREATE_USERDATA(Matrix4);
    SET_METATABLE("Matrix4");

    Vector3* eye = CHECK_USERDATA(L, 1, Vector3);
    Vector3* target = CHECK_USERDATA(L, 2, Vector3);
    Vector3* up = CHECK_USERDATA(L, 3, Vector3);

    Matrix4::createLookAt(*eye, *target, *up, self);
    return 1;
}

static int Matrix4_createPerspective(lua_State* L)
{
    Matrix4* self = CREATE_USERDATA(Matrix4);
    SET_METATABLE("Matrix4");

    float fov = luaL_checknumber(L, 1);
    float aspectRatio = luaL_checknumber(L, 2);
    float zNear = luaL_checknumber(L, 3);
    float zFar = luaL_checknumber(L, 4);

    Matrix4::createPerspective(fov, aspectRatio, zNear, zFar, self);
    return 1;
}

static int Matrix4_createOrthographic(lua_State* L)
{
    Matrix4* self = CREATE_USERDATA(Matrix4);
    SET_METATABLE("Matrix4");

    float width = luaL_checknumber(L, 1);
    float height = luaL_checknumber(L, 2);
    float zNear = luaL_checknumber(L, 3);
    float zFar = luaL_checknumber(L, 4);

    Matrix4::createOrthographic(width, height, zNear, zFar, self);
    return 1;
}

static int Matrix4_createOrthographicRect(lua_State* L)
{
    Matrix4* self = CREATE_USERDATA(Matrix4);
    SET_METATABLE("Matrix4");

    float left = luaL_checknumber(L, 1);
    float right = luaL_checknumber(L, 2);
    float bottom = luaL_checknumber(L, 3);
    float top = luaL_checknumber(L, 4);
    float zNear = luaL_checknumber(L, 5);
    float zFar = luaL_checknumber(L, 6);

    Matrix4::createOrthographicRect(left, right, bottom, top, zNear, zFar, self);
    return 1;
}

static int Matrix4_createBillboard(lua_State* L)
{
    Matrix4* self = CREATE_USERDATA(Matrix4);
    SET_METATABLE("Matrix4");

    Vector3* objectPos = CHECK_USERDATA(L, 1, Vector3);
    Vector3* camPos = CHECK_USERDATA(L, 2, Vector3);
    Vector3* camUp = CHECK_USERDATA(L, 3, Vector3);

    Matrix4::createBillboard(*objectPos, *camPos, *camUp, self);
    return 1;
}

static int Matrix4_createScale(lua_State* L)
{
    Matrix4* self = CREATE_USERDATA(Matrix4);
    SET_METATABLE("Matrix4");

    Vector3* scale = CHECK_USERDATA(L, 1, Vector3);

    Matrix4::createScale(*scale, self);
    return 1;
}

static int Matrix4_createRotation(lua_State* L)
{
    Matrix4* self = CREATE_USERDATA(Matrix4);
    SET_METATABLE("Matrix4");

    Vector3* axis = CHECK_USERDATA(L, 1, Vector3);
    float angle = luaL_checknumber(L, 2);

    Matrix4::createRotation(*axis, angle, self);
    return 1;
}

static int Matrix4_createRotationX(lua_State* L)
{
    Matrix4* self = CREATE_USERDATA(Matrix4);
    SET_METATABLE("Matrix4");

    float angle = luaL_checknumber(L, 1);

    Matrix4::createRotationX(angle, self);
    return 1;
}

static int Matrix4_createRotationY(lua_State* L)
{
    Matrix4* self = CREATE_USERDATA(Matrix4);
    SET_METATABLE("Matrix4");

    float angle = luaL_checknumber(L, 1);

    Matrix4::createRotationY(angle, self);
    return 1;
}

static int Matrix4_createRotationZ(lua_State* L)
{
    Matrix4* self = CREATE_USERDATA(Matrix4);
    SET_METATABLE("Matrix4");

    float angle = luaL_checknumber(L, 1);

    Matrix4::createRotationZ(angle, self);
    return 1;
}

static int Matrix4_createFromEuler(lua_State* L)
{
    Matrix4* self = CREATE_USERDATA(Matrix4);
    SET_METATABLE("Matrix4");

    float yaw = luaL_checknumber(L, 1);
    float pitch = luaL_checknumber(L, 2);
    float roll = luaL_checknumber(L, 3);

    Matrix4::createFromEuler(yaw, pitch, roll, self);
    return 1;
}

static int Matrix4_createTranslation(lua_State* L)
{
    Matrix4* self = CREATE_USERDATA(Matrix4);
    SET_METATABLE("Matrix4");

    Vector3* translation = CHECK_USERDATA(L, 1, Vector3);

    Matrix4::createTranslation(*translation, self);
    return 1;
}


static int Matrix4_set_i(lua_State* L, int ud, int arr)
{
    Matrix4* self = CHECK_USERDATA(L, ud, Matrix4);
    luaL_checktype(L, arr, LUA_TTABLE);

    // Check array length
    int len = luaL_len(L, arr);
    if (len != 16)
        return luaL_error(L, "invalid array length (expected 16, got %d)", len);
    
    for (int i = 1; i < len + 1; ++i)
    {
        lua_pushinteger(L, i); /* push index */
        lua_gettable(L, arr); /* get value at index */

        // Check value type
        int type = lua_type(L, -1);
        if (type != LUA_TNUMBER)
            return luaL_error(L, "invalid value type at array index %d (expected number, got %s)",
                              i, lua_typename(L, type));
        
        self->m[i-1] = lua_tonumber(L, -1);
        lua_pop(L, 1);
    }

    return 0;
}

static int Matrix4_set(lua_State* L)
{
    return Matrix4_set_i(L, 1, 2);
}

static int Matrix4_setIdentity(lua_State* L)
{
    Matrix4* self = CHECK_USERDATA(L, 1, Matrix4);
    self->setIdentity();
    return 0;
}

static int Matrix4_zero(lua_State* L)
{
    Matrix4* self = CHECK_USERDATA(L, 1, Matrix4);
    self->zero();
    return 0;
}

static int Matrix4_determinant(lua_State* L)
{
    Matrix4* self = CHECK_USERDATA(L, 1, Matrix4);
    lua_pushnumber(L, self->determinant());
    return 1;
}

static int Matrix4_invert(lua_State* L)
{
    Matrix4* self = CHECK_USERDATA(L, 1, Matrix4);
    lua_pushboolean(L, self->invert());
    return 1;
}

static int Matrix4_isIdentity(lua_State* L)
{
    Matrix4* self = CHECK_USERDATA(L, 1, Matrix4);
    lua_pushboolean(L, self->isIdentity());
    return 1;
}

static int Matrix4_negate(lua_State* L)
{
    Matrix4* self = CHECK_USERDATA(L, 1, Matrix4);
    self->negate();
    return 0;
}

static int Matrix4_rotate(lua_State* L)
{
    Matrix4* self = CHECK_USERDATA(L, 1, Matrix4);
    Vector3* axis = CHECK_USERDATA(L, 2, Vector3);
    float angle = luaL_checknumber(L, 3);

    self->rotate(*axis, angle);
    return 0;
}

static int Matrix4_rotateX(lua_State* L)
{
    Matrix4* self = CHECK_USERDATA(L, 1, Matrix4);
    float angle = luaL_checknumber(L, 2);

    self->rotateX(angle);
    return 0;
}

static int Matrix4_rotateY(lua_State* L)
{
    Matrix4* self = CHECK_USERDATA(L, 1, Matrix4);
    float angle = luaL_checknumber(L, 2);

    self->rotateY(angle);
    return 0;
}

static int Matrix4_rotateZ(lua_State* L)
{
    Matrix4* self = CHECK_USERDATA(L, 1, Matrix4);
    float angle = luaL_checknumber(L, 2);

    self->rotateZ(angle);
    return 0;
}

static int Matrix4_scale(lua_State* L)
{
    Matrix4* self = CHECK_USERDATA(L, 1, Matrix4);
    Vector3* s = CHECK_USERDATA(L, 2, Vector3);

    self->scale(*s);
    return 0;
}

static int Matrix4_transformVector(lua_State* L)
{
    Matrix4* self = CHECK_USERDATA(L, 1, Matrix4);
    Vector4* v = CHECK_USERDATA(L, 2, Vector4);

    Vector4* res = CREATE_USERDATA(Vector4);
    SET_METATABLE("Vector4");

    self->transformVector(*v, res);
    return 1;
}

static int Matrix4_translate(lua_State* L)
{
    Matrix4* self = CHECK_USERDATA(L, 1, Matrix4);
    Vector3* t = CHECK_USERDATA(L, 2, Vector3);

    self->translate(*t);
    return 0;
}

static int Matrix4_transpose(lua_State* L)
{
    Matrix4* self = CHECK_USERDATA(L, 1, Matrix4);
    self->transpose();
    return 0;
}

static int Matrix4__index(lua_State* L)
{
    Matrix4* self = CHECK_USERDATA(L, 1, Matrix4);
    int type = lua_type(L, 2);

    if (type == LUA_TNUMBER)
    {
        int k = lua_tointeger(L, 2);
        
        if (k < 1 || k > 16)
            lua_pushnil(L);
        else
            lua_pushnumber(L, self->m[k]);
    }
    else if (type == LUA_TSTRING)
    {
        const char* k = lua_tostring(L, 2);
        lua_getfield(L, lua_upvalueindex(1), k);
    }
    else
        lua_pushnil(L);
    
    return 1;
}

static int Matrix4__newindex(lua_State* L)
{
    Matrix4* self = CHECK_USERDATA(L, 1, Matrix4);
    int k = luaL_checkinteger(L, 2);
    float v = luaL_checknumber(L, 3);

    if (k < 1 || k > 16)
        return luaL_error(L, "invalid value index");
    
    self->m[k] = v;
    return 0;
}

static int Matrix4__add(lua_State* L)
{
    OPERATOR_SAME_CLASS_IMPL(Matrix4, +);
}

static int Matrix4__sub(lua_State* L)
{
    OPERATOR_SAME_CLASS_IMPL(Matrix4, -);
}

static int Matrix4__mul(lua_State* L)
{
    Matrix4* lhs = CHECK_USERDATA(L, 1, Matrix4);
    int type = lua_type(L, 2);
    int udType = 0;

    if (type == LUA_TUSERDATA)
    {
        if (luaL_testudata(L, 2, "Matrix4"))
        {
            Matrix4* res = CREATE_USERDATA(Matrix4);
            SET_METATABLE("Matrix4");

            Matrix4* rhs = TO_USERDATA(L, 2, Matrix4);
            *res = *lhs * *rhs;

            return 1;
        }
        else if (luaL_testudata(L, 2, "Vector4"))
        {
            Vector4* res = CREATE_USERDATA(Vector4);
            SET_METATABLE("Vector4");

            Vector4* rhs = TO_USERDATA(L, 2, Vector4);
            *res = *lhs * *rhs;

            return 1;
        }
    }
    else if (type == LUA_TNUMBER)
    {
        Matrix4* res = CREATE_USERDATA(Matrix4);
        SET_METATABLE("Matrix4");

        float rhs = lua_tonumber(L, 2);
        *res = *lhs * rhs;

        return 1;
    }

    return luaL_argerror(L, 2, "expected Matrix4, Vector4 or number");
}

static const luaL_Reg Matrix4_statics[] = {
    {"new", Matrix4_new},
    {"createLookAt", Matrix4_createLookAt},
    {"createPerspective", Matrix4_createPerspective},
    {"createOrthographic", Matrix4_createOrthographic},
    {"createOrthographicRect", Matrix4_createOrthographicRect},
    {"createBillboard", Matrix4_createBillboard},
    {"createScale", Matrix4_createScale},
    {"createRotation", Matrix4_createRotation},
    {"createRotationX", Matrix4_createRotationX},
    {"createRotationY", Matrix4_createRotationY},
    {"createRotationZ", Matrix4_createRotationZ},
    {"createFromEuler", Matrix4_createFromEuler},
    {"createTranslation", Matrix4_createTranslation},
    {NULL, NULL}
};

static const luaL_Reg Matrix4_methods[] = {
    {"set", Matrix4_set},
    {"setIdentity", Matrix4_setIdentity},
    {"zero", Matrix4_zero},
    {"determinant", Matrix4_determinant},
    {"invert", Matrix4_invert},
    {"isIdentity", Matrix4_isIdentity},
    {"negate", Matrix4_negate},
    {"rotate", Matrix4_rotate},
    {"rotateX", Matrix4_rotateX},
    {"rotateY", Matrix4_rotateY},
    {"rotateZ", Matrix4_rotateZ},
    {"scale", Matrix4_scale},
    {"transformVector", Matrix4_transformVector},
    {"translate", Matrix4_translate},
    {"transpose", Matrix4_transpose},
    {NULL, NULL}
};

static const luaL_Reg Matrix4_meta[] = {
    {"__index", Matrix4__index},
    {"__newindex", Matrix4__newindex},
    {"__add", Matrix4__add},
    {"__sub", Matrix4__sub},
    {"__mul", Matrix4__mul},
    {NULL, NULL}
};

/***********  ImageData  ***********/

static int ImageData__index(lua_State* L)
{
    ImageData* self = CHECK_USERDATA(L, 1, ImageData);
    const char* k = luaL_checkstring(L, 2);
    
    if (strcmp(k, "width") == 0)
        lua_pushinteger(L, self->width);
    else if (strcmp(k, "height") == 0)
        lua_pushinteger(L, self->height);
    else
        lua_pushnil(L);

    return 1;
}

static const luaL_Reg ImageData_meta[] = {
    {"__index", ImageData__index},
    {NULL, NULL}
};

/***********  Enums  ***********/

static const Enum DataType_enum[] = {
    {"FLOAT", FLOAT},
    {"INT", INT},
    {"UINT", UINT},
    {"SHORT", SHORT},
    {"USHORT", USHORT},
    {"BYTE", BYTE},
    {"UBYTE", UBYTE},
    {"VECTOR2", VECTOR2},
    {"VECTOR3", VECTOR3},
    {"VECTOR4", VECTOR4},
    {"COLOR3", COLOR3},
    {"COLOR4", COLOR4},
    {"MATRIX4", MATRIX4},
    {"IMAGEDATA", IMAGEDATA}
};

static const Enum BufferType_enum[] = {
    {"ARRAY_BUFFER", ARRAY_BUFFER},
    {"ELEMENT_ARRAY_BUFFER", ELEMENT_ARRAY_BUFFER}
};

static const Enum PrimitiveType_enum[] = {
    {"POINTS", POINTS},
    {"TRIANGLES", TRIANGLES}
};

static const Enum Capability_enum[] = {
    {"BLEND", BLEND},
    {"DEPTH_TEST", DEPTH_TEST},
    {"CULL_FACE", CULL_FACE}
};

static const Enum Faces_enum[] = {
    {"FRONT", FRONT},
    {"BACK", BACK},
    {"FRONT_AND_BACK", FRONT_AND_BACK}
};

static const Enum Winding_enum[] = {
    {"CCW", CCW},
    {"CW", CW}
};

/*******************************/

void openLibs(lua_State* L)
{
    REGISTER_CLASS_META(Vector2);
    REGISTER_CLASS_META(Vector3);
    REGISTER_CLASS_META(Vector4);

    REGISTER_CLASS_META(Color3);
    REGISTER_CLASS_META(Color4);

    REGISTER_CLASS_META(Matrix4);

    // ImageData (no methods, read-only)
    luaL_newmetatable(L, "ImageData");
    luaL_setfuncs(L, ImageData_meta, 0);
    lua_pop(L, 1);

    REGISTER_ENUM(DataType);
    REGISTER_ENUM(BufferType);
    REGISTER_ENUM(PrimitiveType);
    REGISTER_ENUM(Capability);
    REGISTER_ENUM(Faces);
    REGISTER_ENUM(Winding);
}

}
}