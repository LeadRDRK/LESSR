#pragma once
#include <cstdint>
#include <vector>
#include <unordered_set>
#include "Enums.h"
#include "VertexAttribute.h"
#include <LESSR/Color4.h>
#include <LESSR/Vector3.h>
#include <LESSR/Program.h>
#include <LESSR/DefaultProgram.h>

namespace LESSR
{

class Context
{
public:
    static uint8_t* createFramebuffer(uint32_t width, uint32_t height);
    static float* createDepthBuffer(uint32_t width, uint32_t height);

    void viewport(uint32_t width, uint32_t height);
    void clearColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    void clear();

    void bindFramebuffer(uint8_t* fb);
    void bindDepthBuffer(float* db);

    void enable(Capability cap);
    void disable(Capability cap);

    void cullFace(Faces face);
    void frontFace(Winding mode);

    void useProgram(Program* program);

    void bindBuffer(const void* ptr, BufferType type);
    void vertexLayout(const VertexLayout& layout);

    void uniform(DataType type, const std::string& name, const void* data);
    void bindTexture(const std::string& name, const ImageData* texture);

    void drawArrays(PrimitiveType mode, uint32_t first, uint32_t count);
    void drawElements(PrimitiveType mode, uint32_t first, uint32_t count);

private:
    void blendPixel(std::size_t pOffset, const Color4& color);
    void plotPixel(const Vector4& pos, const Color4& color);

    Vector4 ndcToViewport(const Vector4& p);

    bool isFaceCulled(int vertCount);

    void drawPoint();
    void drawTriangle();

    void drawPrimitive(PrimitiveType type);

    uint32_t width = 0,
             height = 0;
    uint8_t* fb = nullptr;
    float* db = nullptr;

    uint8_t _clearColor[4] = {};

    bool enabled[3] = {};
    Faces _cullFace = BACK;
    Winding _frontFace = CCW;

    const uint32_t* indexBuffer;

    Program* program = &DefaultProgram::instance;
    ShaderContext shaderCtx;

};

}