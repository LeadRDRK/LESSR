#pragma once
#include <LESSR/Program.h>

namespace LESSR
{

class DefaultProgram : public Program
{
public:
    void vertexShader(ShaderContext& ctx) override;
    void fragmentShader(ShaderContext& ctx, const Vector4& fragCoord) override;

    static DefaultProgram instance;

};

}