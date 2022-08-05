#include <LESSR/Context.h>
#include <LESSR/MathUtils.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define RES_W 512
#define RES_H 512

using namespace LESSR;

struct Vertex
{
    Vector2 position;
    Color3 color;
    char __padding;
};

static const VertexLayout vertexLayout = {
    {"a_position", {VECTOR2, sizeof(Vertex), 0}},
    {"a_color",    {COLOR3,  sizeof(Vertex), sizeof(Vector2)}}
};

static const Vertex vertices[] = {
    {Vector2( 0,    0.5), Color3(255, 0, 0)},
    {Vector2(-0.5, -0.5), Color3(0, 255, 0)},
    {Vector2( 0.5, -0.5), Color3(0, 0, 255)}
};

class SProgram : public LESSR::Program
{
public:
    void vertexShader(LESSR::ShaderContext& ctx) override
    {
        auto a_position = ctx.attribute<Vector2>(VECTOR2, "a_position");
        auto a_color    = ctx.attribute<Color3> (COLOR3,  "a_color");
        auto v_color    = ctx.varying  <Color3> (COLOR3,  "v_color");

        ctx.outPosition(*a_position);
        *v_color = *a_color;
    }

    void fragmentShader(LESSR::ShaderContext& ctx, const Vector4& fragCoord) override
    {
        auto v_color = ctx.varying<Color3>(COLOR3, "v_color");
        ctx.outColor(*v_color);
    }

};

int main(int argc, char** argv)
{
    Context ctx;

    uint8_t* fb = ctx.createFramebuffer(RES_W, RES_H);
    ctx.bindFramebuffer(fb);
    ctx.viewport(RES_W, RES_H);
    ctx.clear();

    SProgram program;
    ctx.useProgram(&program);

    ctx.bindBuffer(vertices, ARRAY_BUFFER);
    ctx.vertexLayout(vertexLayout);

    ctx.drawArrays(TRIANGLES, 0, sizeof(vertices)/sizeof(vertices[0]));
    stbi_write_png("triangle.png", RES_W, RES_H, 4, fb, RES_W*4);

    delete[] fb;
    return 0;
}