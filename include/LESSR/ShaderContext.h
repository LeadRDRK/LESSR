#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include <LESSR/VertexAttribute.h>
#include <LESSR/Vector4.h>
#include <LESSR/Color4.h>
#include <LESSR/ImageData.h>

namespace LESSR
{

class ShaderContext
{
public:
    template<class T = void>
    const T* attribute(DataType type, const std::string& name) const;

    template<class T = void>
    T* varying(DataType type, const std::string& name);

    template<class T = void>
    const T* uniform(DataType type, const std::string& name) const;

    void outPosition(const Vector4& pos);
    void outColor(const Color4& color);

    const VertexLayout& layout() const;

    uint32_t vertexNum() const;

private:
    const void* attribute_i(DataType type, const std::string& name) const;
    void* varying_i(DataType type, const std::string& name);
    const void* uniform_i(DataType type, const std::string& name) const;

    void nextVertex();
    void nextPrimitive();
    void prepareVaryings(const float* alpha, uint32_t count);

    void reset();

    const void* vertexData;
    VertexLayout vertexLayout;

    struct Varying
    {
        DataType type;
        std::vector<std::vector<char>> stops;
        std::vector<char> data; // per-fragment data
    };

    struct Uniform
    {
        DataType type;
        const void* data;
    };

    std::unordered_map<std::string, Varying> varyings;
    std::unordered_map<std::string, Uniform> uniforms;
    
    Vector4 _outPosition;
    Color4 _outColor;

    std::vector<Vector4> positions;

    uint32_t num = 0;
    bool isFragment = false;
    bool isFirst = true;

    friend class Context;

};

template<class T>
inline const T* ShaderContext::attribute(DataType type, const std::string& name) const
{
    return reinterpret_cast<const T*>(attribute_i(type, name));
}

template<class T>
inline T* ShaderContext::varying(DataType type, const std::string& name)
{
    return reinterpret_cast<T*>(varying_i(type, name));
}

template<class T>
inline const T* ShaderContext::uniform(DataType type, const std::string& name) const
{
    return reinterpret_cast<const T*>(uniform_i(type, name));
}

}