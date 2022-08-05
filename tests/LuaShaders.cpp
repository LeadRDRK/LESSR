#include <LESSR/Context.h>
#include <LESSR/MathUtils.h>
#include <LESSR/LuaProgram.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define RES_W 512
#define RES_H 512

using namespace LESSR;

struct Vertex
{
    Vector3 position;
    Vector2 texCoord;
};

static const VertexLayout vertexLayout = {
    {"a_position", {VECTOR3, sizeof(Vertex), 0}},
    {"a_texCoord", {VECTOR2, sizeof(Vertex), sizeof(Vector3)}}
};

/*  rough topdown visualization
            ^ x
            |
            |
            b1
            /\
          /    \
        /   o    \
      /            \
    /________________\ ------> z
   b2                 b3
*/

static const Vertex vertices[] = {
    {Vector3( 0,    0.5,  0),   Vector2(0.5, 0)}, // o  - 0
    {Vector3( 0.5, -0.5,  0),   Vector2(1, 1)},   // b1 - 1
    {Vector3( 0.5, -0.5,  0),   Vector2(0, 1)},   // b1 - 2
    {Vector3(-0.5, -0.5, -0.5), Vector2(1, 1)},   // b2 - 3
    {Vector3(-0.5, -0.5, -0.5), Vector2(0, 1)},   // b2 - 4
    {Vector3(-0.5, -0.5,  0.5), Vector2(1, 1)},   // b3 - 5
    {Vector3(-0.5, -0.5,  0.5), Vector2(0, 1)},   // b3 - 6
    {Vector3( 0.5, -0.5,  0),   Vector2(0.5, 0)}, // b1 - 7
};

static const uint32_t indices[] = {
    0, 2, 3,
    0, 4, 5,
    0, 6, 1,
    7, 6, 3
};

static const char* shadersScript =
    "function vertexShader()\n"
        "local a_position = attribute(DataType.VECTOR3, 'a_position')\n"
        "local a_texCoord = attribute(DataType.VECTOR2, 'a_texCoord')\n"
        "local u_vpMatrix = uniform  (DataType.MATRIX4, 'u_vpMatrix')\n"
        "varying(DataType.VECTOR2, 'v_texCoord', a_texCoord)\n"
        "return u_vpMatrix * Vector4.new(a_position)\n"
    "end\n"

    "function fragmentShader(fragCoord)\n"
        "local v_texCoord = varying(DataType.VECTOR2,  'v_texCoord')\n"
        "local u_texture  = uniform(DataType.IMAGEDATA, 'u_texture')\n"
        "return texture(u_texture, v_texCoord)\n"
    "end";

int main(int argc, char** argv)
{
    if (argc < 3)
    {
        std::cout << "usage: " << argv[0] << " <texture> <angle>" << std::endl;
        return 1;
    }

    ImageData image;
    int t1, t2, comp;
    image.pixels = stbi_load(argv[1], &t1, &t2, &comp, STBI_rgb_alpha);
    if (!image.pixels)
    {
        std::cout << "failed to load texture" << std::endl;
        return 1;
    }
    image.width = t1;
    image.height = t2;

    char* ptr = nullptr;
    int angle = strtol(argv[2], &ptr, 10);
    if (ptr == nullptr || *ptr)
    {
        std::cout << "invalid angle value" << std::endl;
        return 1;
    }

    Matrix4 projMatrix;
    Matrix4::createPerspective(90.f, (float)RES_W/RES_H, -1.f, 1.f, &projMatrix);

    float radAngle = MATH_DEG_TO_RAD(angle);
    Matrix4 viewMatrix;
    Matrix4::createFromEuler(radAngle, radAngle * 2, 0, &viewMatrix);

    Matrix4 vpMatrix;
    Matrix4::multiply(viewMatrix, projMatrix, &vpMatrix);

    Context ctx;
    ctx.enable(BLEND);
    ctx.enable(DEPTH_TEST);
    ctx.enable(CULL_FACE);

    ctx.cullFace(BACK);
    ctx.frontFace(CCW);

    uint8_t* fb = ctx.createFramebuffer(RES_W, RES_H);
    float* db = ctx.createDepthBuffer(RES_W, RES_H);
    ctx.bindFramebuffer(fb);
    ctx.bindDepthBuffer(db);
    ctx.viewport(RES_W, RES_H);
    ctx.clear();

    LuaProgram program;
    program.init();
    program.loadString(shadersScript);
    ctx.useProgram(&program);

    ctx.bindBuffer(vertices, ARRAY_BUFFER);
    ctx.bindBuffer(indices, ELEMENT_ARRAY_BUFFER);
    ctx.vertexLayout(vertexLayout);

    ctx.uniform(MATRIX4, "u_vpMatrix", &vpMatrix);
    ctx.bindTexture("u_texture", &image);

    ctx.drawElements(TRIANGLES, 0, sizeof(indices)/sizeof(indices[0]));
    stbi_write_png("luashaders.png", RES_W, RES_H, 4, fb, RES_W*4);

    stbi_image_free(image.pixels);
    delete[] fb;
    delete[] db;
    return 0;
}