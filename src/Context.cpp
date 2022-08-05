#include <LESSR/Context.h>
#include <LESSR/MathUtils.h>
#include <cstring>
#include <limits>

#define FB_SIZE(width, height) (width) * (height) * 4
#define DO_FRAGMENT(alpha, count, coord) shaderCtx.prepareVaryings(alpha, count); program->fragmentShader(shaderCtx, coord)

namespace LESSR
{

uint8_t* Context::createFramebuffer(uint32_t width, uint32_t height)
{
    return new uint8_t[FB_SIZE(width, height)];
}

float* Context::createDepthBuffer(uint32_t width, uint32_t height)
{
    return new float[width * height];
}

void Context::viewport(uint32_t w, uint32_t h)
{
    width = w;
    height = h;
}

void Context::clearColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    _clearColor[0] = r;
    _clearColor[1] = g;
    _clearColor[2] = b;
    _clearColor[3] = a;
}

void Context::clear()
{
    for (size_t i = 0; i < FB_SIZE(width, height); ++i)
        fb[i] = *(_clearColor + (i % 4));
    
    if (db)
        memset(db, 0, sizeof(float) * width * height);
}

void Context::bindFramebuffer(uint8_t* _fb)
{
    fb = _fb;
}

void Context::bindDepthBuffer(float* _db)
{
    db = _db;
}

void Context::enable(Capability cap)
{
    enabled[cap] = true;
}

void Context::disable(Capability cap)
{
    enabled[cap] = false;
}

void Context::cullFace(Faces face)
{
    _cullFace = face;
}

void Context::frontFace(Winding mode)
{
    _frontFace = mode;
}

void Context::useProgram(Program* p)
{
    program = p ? p : &DefaultProgram::instance;
}

void Context::bindBuffer(const void* ptr, BufferType type)
{
    if (type == ARRAY_BUFFER)
        shaderCtx.vertexData = ptr;
    else
        indexBuffer = reinterpret_cast<const uint32_t*>(ptr);
}

void Context::vertexLayout(const VertexLayout& layout)
{
    shaderCtx.vertexLayout = layout;
}

void Context::uniform(DataType type, const std::string& name, const void* data)
{
    shaderCtx.uniforms[name] = { type, data };
}

void Context::bindTexture(const std::string& name, const ImageData* texture)
{
    uniform(IMAGEDATA, name, texture);
}

void Context::blendPixel(std::size_t pOffset, const Color4& color)
{
    if (color.a == 0.f) return;
    uint8_t* p = fb + pOffset*4;
    float a = 255.f/color.a;

    Color3 v0(p);
    v0 *= (1.f - a);

    Color3 v1(color);
    v1 *= a;

    Color4 v(v0 + v1, std::max(p[3] + color.a, 255));
    memcpy(p, &v, sizeof(Color4));
}

void Context::plotPixel(const Vector4& pos, const Color4& color)
{
    uint32_t x = std::floor(pos.x);
    uint32_t y = std::floor(pos.y);
    if (x > width || y > height) return;

    size_t p = x + y * width;

    if (enabled[DEPTH_TEST])
    {
        if (pos.z > db[p])
            db[p] = pos.z;
        else
            return;
    }

    if (enabled[BLEND])
        blendPixel(p, color);
    else
        memcpy(fb + p*4, &color, sizeof(Color4));
}

Vector4 Context::ndcToViewport(const Vector4& p)
{
    return Vector3(((p.x + 1.f) / 2.f) * width,
                   ((-p.y + 1.f) / 2.f) * height, p.z);
}

bool Context::isFaceCulled(int vertCount)
{
    if (vertCount < 3) return false;
    if (_cullFace == FRONT_AND_BACK) return true;

    const auto& p = shaderCtx.positions;
    float dArea = 0.f;
    for (int i = 0; i < vertCount; ++i)
    {
        int x = (i != vertCount - 1) ? i+1 : 0;
        dArea += p[i].x * p[x].y - p[i].y * p[x].x;
    }
    if (dArea == 0.f) return true;

    Winding cullWinding = (_cullFace == FRONT) ? _frontFace : (_frontFace == CCW ? CW : CCW);
    switch (cullWinding)
    {
    case CCW: return dArea < 0;
    case CW:  return dArea > 0;
    }

    return true;
}

void Context::drawPoint()
{
    const auto& v = shaderCtx.positions[0];
    Vector4 pos = ndcToViewport(v);

    static const float alpha = 1.f;
    DO_FRAGMENT(&alpha, 1, pos);
    plotPixel(pos, shaderCtx._outColor);
}

static Vector3 barycentric(const Vector2 pts[3], const Vector3& P) {
    Vector3 u1 = Vector3(pts[2].x-pts[0].x, pts[1].x-pts[0].x, pts[0].x-P.x);
    Vector3 u2 = Vector3(pts[2].y-pts[0].y, pts[1].y-pts[0].y, pts[0].y-P.y);
    Vector3 u;   u1.cross(u2, &u);
    if (std::abs(u.z)<1) return Vector3(-1,1,1);
    return Vector3(1.f-(u.x+u.y)/u.z, u.y/u.z, u.x/u.z); 
}

void Context::drawTriangle()
{
    const auto& p0 = shaderCtx.positions[0];
    const auto& p1 = shaderCtx.positions[1];
    const auto& p2 = shaderCtx.positions[2];
    
    Vector4 pts[3]  = { ndcToViewport(p0), ndcToViewport(p1), ndcToViewport(p2) };
    Vector2 pts2[3] = { pts[0]/pts[0].w, pts[1]/pts[1].w, pts[2]/pts[2].w };

    Vector2 bboxmin(width - 1,  height - 1); 
    Vector2 bboxmax(0, 0); 
    Vector2 clamp(width - 1, height - 1); 
    for (int i = 0; i < 3; i++) { 
        bboxmin.x = MATH_CLAMP(bboxmin.x, 0.f, pts2[i].x);
        bboxmin.y = MATH_CLAMP(bboxmin.y, 0.f, pts2[i].y);

        bboxmax.x = MATH_CLAMP(bboxmax.x, clamp.x, pts2[i].x);
        bboxmax.y = MATH_CLAMP(bboxmax.y, clamp.y, pts2[i].y);
    }

#pragma omp parallel for
    for (int x=(int)bboxmin.x; x<=(int)bboxmax.x; x++)
    {
        for (int y=(int)bboxmin.y; y<=(int)bboxmax.y; y++)
        {
            Vector3 pos(x, y, 0);
            Vector3 bcScreen = barycentric(pts2, pos);
            if (bcScreen.x < 0 || bcScreen.y < 0 || bcScreen.z < 0) continue;

            Vector3 bcClip(bcScreen.x/pts[0].w, bcScreen.y/pts[1].w, bcScreen.z/pts[2].w);
            bcClip /= bcClip.x + bcClip.y + bcClip.z;

            pos.z = Vector3(p0.z, p1.z, p2.z).dot(bcClip);
            size_t px = pos.x+pos.y*width;

#pragma omp critical
            {
                DO_FRAGMENT(&bcClip.x, 3, pos);
                plotPixel(pos, shaderCtx._outColor);
            }
        }
    }
}

void Context::drawPrimitive(PrimitiveType type)
{
    shaderCtx.isFragment = true;
    switch (type)
    {
    case POINTS:    drawPoint();    break;
    case TRIANGLES: drawTriangle(); break;
    }
    shaderCtx.isFragment = false;
    shaderCtx.nextPrimitive();
}

static int primitiveVertCount(PrimitiveType type)
{
    switch (type)
    {
    case POINTS: return 1;
    case TRIANGLES: return 3;
    }

    return 0;
}

void Context::drawArrays(PrimitiveType mode, uint32_t first, uint32_t count)
{
    int pVertCount = primitiveVertCount(mode);
    if (count % pVertCount != 0) return;

    shaderCtx.reset();

    int last = first + count;
    for (uint32_t i = first; i < last; i += pVertCount)
    {
        for (int x = 0; x < pVertCount; ++x) {
            shaderCtx.num = i + x;
            program->vertexShader(shaderCtx);
            shaderCtx.nextVertex();
        }

        if (enabled[CULL_FACE] && isFaceCulled(pVertCount))
        {
            shaderCtx.nextPrimitive();
            continue;
        }

        drawPrimitive(mode);
    }
}

void Context::drawElements(PrimitiveType mode, uint32_t first, uint32_t count)
{
    int pVertCount = primitiveVertCount(mode);
    if (count % pVertCount != 0) return;
    
    shaderCtx.reset();

    int last = first + count;
    for (uint32_t i = first; i < last; i += pVertCount)
    {
        for (int x = 0; x < pVertCount; ++x) {
            shaderCtx.num = indexBuffer[i + x];
            program->vertexShader(shaderCtx);
            shaderCtx.nextVertex();
        }

        if (enabled[CULL_FACE] && isFaceCulled(pVertCount))
        {
            shaderCtx.nextPrimitive();
            continue;
        }

        drawPrimitive(mode);
    }
}

}