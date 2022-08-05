#include <LESSR/Program.h>
#include <LESSR/MathUtils.h>

namespace LESSR
{

Color4 Program::texture(const ImageData* image, const Vector2& coords)
{
    uint32_t x = MATH_CLAMP(coords.x, 0.f, 1.f) * (image->width-1);
    uint32_t y = MATH_CLAMP(coords.y, 0.f, 1.f) * (image->height-1);

    size_t p = (x + y * image->width) * 4;
    return Color4(image->pixels + p);
}

}