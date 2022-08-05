#include <LESSR/Context.h>
#include "Suzanne.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define VERTEX_COUNT (sizeof(Suzanne)/sizeof(Suzanne[0]))/3
#define RES_W 512
#define RES_H 512

using namespace LESSR;

class SProgram : public LESSR::Program
{
public:
    void vertexShader(LESSR::ShaderContext& ctx) override
    {
        auto pos = ctx.attribute<Vector3>(VECTOR3, "position");
        ctx.outPosition(*pos);
    }

    void fragmentShader(LESSR::ShaderContext& ctx, const Vector4& fragCoord) override
    {
        uint8_t v = std::max(0.f, std::min(255 * fragCoord.z, 255.f));
        ctx.outColor(Color4(v, v, v, 255));
    }

};

int main(int argc, char** argv)
{
    Context ctx;
    ctx.enable(BLEND);
    ctx.enable(DEPTH_TEST);

    uint8_t* fb = ctx.createFramebuffer(RES_W, RES_H);
    float* db = ctx.createDepthBuffer(RES_W, RES_H);
    ctx.bindFramebuffer(fb);
    ctx.bindDepthBuffer(db);
    ctx.viewport(RES_W, RES_H);
    ctx.clear();

    SProgram program;
    ctx.useProgram(&program);

    ctx.bindBuffer(Suzanne, ARRAY_BUFFER);
    ctx.vertexLayout({{"position", {VECTOR3, sizeof(Vector3), 0}}});
    ctx.drawArrays(TRIANGLES, 0, VERTEX_COUNT);

    stbi_write_png("suzanne.png", RES_W, RES_H, 4, fb, RES_W*4);

    delete[] fb;
    delete[] db;
    return 0;
}