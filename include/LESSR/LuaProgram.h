#pragma once
#include <LESSR/Program.h>

typedef struct lua_State lua_State;

namespace LESSR
{

class LuaProgram : public Program
{
public:
    LuaProgram(lua_State* L = nullptr);
    ~LuaProgram();

    bool init();
    bool loadString(const char* script);
    bool loadFile(const char* path);

    lua_State* getState() const;

    void vertexShader(ShaderContext& ctx) override;
    void fragmentShader(ShaderContext& ctx, const Vector4& fragCoord) override;

private:
    lua_State* L;
    ShaderContext* currentCtx;

};

}