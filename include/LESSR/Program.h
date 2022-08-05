#pragma once
#include <LESSR/ShaderContext.h>

namespace LESSR
{

class Program
{
public:
    virtual void vertexShader(ShaderContext& ctx) = 0;
    virtual void fragmentShader(ShaderContext& ctx, const Vector4& fragCoord) = 0;

    static Color4 texture(const ImageData* image, const Vector2& coords);

};

}