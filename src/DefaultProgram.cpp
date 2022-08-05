#include <LESSR/DefaultProgram.h>
#include <LESSR/Vector3.h>
#include <LESSR/Color4.h>

namespace LESSR
{

void DefaultProgram::vertexShader(ShaderContext& ctx)
{
    auto pos = ctx.attribute<Vector3>(VECTOR3, "position");
    if (!pos) return;
    ctx.outPosition(*pos);
}

void DefaultProgram::fragmentShader(ShaderContext& ctx, const Vector4&)
{
    ctx.outColor(Color4());
}

DefaultProgram DefaultProgram::instance;

}