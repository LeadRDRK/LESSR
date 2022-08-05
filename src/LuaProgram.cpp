#include <LESSR/LuaProgram.h>
#include <LESSR/LuaLib.h>
#include <lua.hpp>
#include "LuaUtils.h"
#include <cstring>

#define GET_CONTEXT(L) *reinterpret_cast<ShaderContext**>(lua_touserdata(L, lua_upvalueindex(1)));

namespace LESSR
{

static void get_ud_name_size(DataType type, const char*& tname, size_t& size)
{
    switch (type)
    {
    case VECTOR2:
        size = sizeof(Vector2);
        tname = "Vector2";
        break;

    case VECTOR3:
        size = sizeof(Vector3);
        tname = "Vector3";
        break;

    case VECTOR4:
        size = sizeof(Vector4);
        tname = "Vector4";
        break;

    case COLOR3:
        size = sizeof(Color3);
        tname = "Color3";
        break;

    case COLOR4:
        size = sizeof(Color4);
        tname = "Color4";
        break;

    case MATRIX4:
        size = sizeof(Matrix4);
        tname = "Matrix4";
        break;

    case IMAGEDATA:
        size = sizeof(ImageData);
        tname = "ImageData";
        break;

    default:
        size = 0;
        break;

    }
}

static int to_lua_value(lua_State* L, const void* ptr, DataType type)
{
    const char* tname;
    size_t size;

    switch (type)
    {
    case FLOAT:
        lua_pushnumber(L, *(float*)ptr);
        break;

    case INT:
        lua_pushinteger(L, *(int*)ptr);
        break;

    case UINT:
        lua_pushinteger(L, *(unsigned int*)ptr);
        break;

    case SHORT:
        lua_pushinteger(L, *(short*)ptr);
        break;

    case USHORT:
        lua_pushinteger(L, *(unsigned short*)ptr);
        break;

    case BYTE:
        lua_pushinteger(L, *(char*)ptr);
        break;

    case UBYTE:
        lua_pushinteger(L, *(unsigned char*)ptr);
        break;

    default:
        get_ud_name_size(type, tname, size);
        break;

    }

    if (size)
    {
        void* dst = lua_newuserdatauv(L, size, 0);
        SET_METATABLE(tname);
        memcpy(dst, ptr, size);
    }

    return 1;
}

static int ctx_attribute(lua_State* L)
{
    ShaderContext* ctx = GET_CONTEXT(L);
    DataType type = (DataType)luaL_checkinteger(L, 1);
    const char* name = luaL_checkstring(L, 2);

    const void* ptr = ctx->attribute(type, name);
    if (!ptr) return 0;

    return to_lua_value(L, ptr, type);
}

static int ctx_uniform(lua_State* L)
{
    ShaderContext* ctx = GET_CONTEXT(L);
    DataType type = (DataType)luaL_checkinteger(L, 1);
    const char* name = luaL_checkstring(L, 2);

    const void* ptr = ctx->uniform(type, name);
    if (!ptr) return 0;

    return to_lua_value(L, ptr, type);
}

static int ctx_varying(lua_State* L)
{
    ShaderContext* ctx = GET_CONTEXT(L);
    DataType type = (DataType)luaL_checkinteger(L, 1);
    const char* name = luaL_checkstring(L, 2);

    void* ptr = ctx->varying(type, name);
    if (!ptr) return 0;

    if (lua_gettop(L) > 2) /* assign value */
    {
        const char* tname;
        size_t size;
        get_ud_name_size(type, tname, size);

        if (size == 0)
            return luaL_error(L, "data type %d cannot be used for varying", type);

        void* val = luaL_checkudata(L, 3, tname);
        memcpy(ptr, val, size);

        return 0;
    }
    else /* get value */
        return to_lua_value(L, ptr, type);
}

static int ctx_vertexNum(lua_State* L)
{
    ShaderContext* ctx = GET_CONTEXT(L);
    lua_pushinteger(L, ctx->vertexNum());
    return 1;
}

static const luaL_Reg ctx_funcs[] = {
    {"attribute", ctx_attribute},
    {"uniform",   ctx_uniform},
    {"varying",   ctx_varying},
    {"vertexNum", ctx_vertexNum},
    {NULL, NULL}
};

// Texture function
static int program_texture(lua_State* L)
{
    ImageData* image = CHECK_USERDATA(L, 1, ImageData);
    Vector2* coords = CHECK_USERDATA(L, 2, Vector2);

    Color4* res = CREATE_USERDATA(Color4);
    SET_METATABLE("Color4");

    *res = Program::texture(image, *coords);
    return 1;
}

LuaProgram::LuaProgram(lua_State* L)
: L(L)
{
}

LuaProgram::~LuaProgram()
{
    if (L) lua_close(L);
}

bool LuaProgram::init()
{
    if (L == nullptr)
    {
        L = luaL_newstate();
        if (L == nullptr)
            return false;
        
        luaL_openlibs(L);
        LuaLib::openLibs(L);
    }

    lua_pushglobaltable(L);

    // context functions
    lua_pushlightuserdata(L, &currentCtx);
    luaL_setfuncs(L, ctx_funcs, 1); // upvalue is pointer to pointer of current context

    // texture function
    lua_pushstring(L, "texture");
    lua_pushcfunction(L, program_texture);
    lua_rawset(L, -3);

    lua_pop(L, 1);

    return true;
}

bool LuaProgram::loadString(const char* script)
{
    return luaL_dostring(L, script) == LUA_OK;
}

bool LuaProgram::loadFile(const char* path)
{
    return luaL_dofile(L, path) == LUA_OK;
}

lua_State* LuaProgram::getState() const
{
    return L;
}

void LuaProgram::vertexShader(ShaderContext& ctx)
{
    currentCtx = &ctx;
    lua_getglobal(L, "vertexShader");
    if (lua_pcall(L, 0, 1, 0) != LUA_OK)
    {
        lua_writestringerror("%s\n", lua_tostring(L, -1));
        lua_pop(L, 1);
    }
    Vector4* out = TEST_USERDATA(L, -1, Vector4);
    if (out) ctx.outPosition(*out);
}

void LuaProgram::fragmentShader(ShaderContext& ctx, const Vector4& fragCoord)
{
    currentCtx = &ctx;
    lua_getglobal(L, "fragmentShader");
    
    // fragCoord is used as argument
    Vector4* fc = CREATE_USERDATA(Vector4);
    SET_METATABLE("Vector4");
    *fc = fragCoord;
    
    if (lua_pcall(L, 1, 1, 0) != LUA_OK)
    {
        lua_writestringerror("%s\n", lua_tostring(L, -1));
        lua_pop(L, 1);
    }
    Color4* out = TEST_USERDATA(L, -1, Color4);
    if (out) ctx.outColor(*out);
}

}