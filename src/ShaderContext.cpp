#include "LESSR/MathUtils.h"
#include <LESSR/ShaderContext.h>
#include <cstring>

namespace LESSR
{

const void* ShaderContext::attribute_i(DataType type, const std::string& name) const
{
    if (isFragment) return nullptr;

    auto it = vertexLayout.find(name);
    if (it != vertexLayout.end())
    {
        auto& attrib = it->second;
        if (attrib.type == type)
            return (char*)vertexData + attrib.stride * num + attrib.offset;
    }
    return nullptr;
}

static size_t varyingTypeSize(DataType type)
{
    switch (type)
    {
    case VECTOR2:
        return sizeof(Vector2);
    
    case VECTOR3:
        return sizeof(Vector3);

    case VECTOR4:
        return sizeof(Vector4);
    
    case COLOR3:
        return sizeof(Color3);
    
    case COLOR4:
        return sizeof(Color4);
    
    default:
        return 0;

    }
}

void* ShaderContext::varying_i(DataType type, const std::string& name)
{
    auto it = varyings.find(name);
    if (it != varyings.end())
    {
        if (it->second.type != type) return nullptr;
        auto& data = isFragment ? it->second.data : it->second.stops.back();
        return &data[0];
    }
    else if (isFirst) // Only allow varying definition on first vertex iteration
    {
        size_t size = varyingTypeSize(type);
        if (!size) return nullptr;

        // Add to list of varyings
        it = varyings.insert({name, {type}}).first;
        auto& stops = it->second.stops;
        // Allocate data buffers
        stops.emplace_back(std::vector<char>(size));
        it->second.data = std::vector<char>(size);
        return &(stops.back()[0]);
    }

    return nullptr;
}

const void* ShaderContext::uniform_i(DataType type, const std::string& name) const
{
    auto it = uniforms.find(name);
    if (it != uniforms.end())
    {
        if (it->second.type == type)
            return it->second.data;
    }
    
    return nullptr;
}

void ShaderContext::outPosition(const Vector4& pos)
{
    if (isFragment) return;
    _outPosition = pos;
}

void ShaderContext::outColor(const Color4& color)
{
    if (!isFragment) return;
    _outColor = color;
}

const VertexLayout& ShaderContext::layout() const
{
    return vertexLayout;
}

uint32_t ShaderContext::vertexNum() const
{
    return num;
}

void ShaderContext::nextVertex()
{
    // Add position
    positions.push_back(_outPosition);
    _outPosition = Vector3();

    // Add varying stop for next vertex
    for (auto& it : varyings)
    {
        auto& varying = it.second;
        size_t size = varyingTypeSize(varying.type);
        varying.stops.emplace_back(std::vector<char>(size));
    }

    isFirst = false;
}

void ShaderContext::nextPrimitive()
{
    // Clear positions and varying stops
    positions.clear();
    for (auto& it : varyings)
    {
        auto& varying = it.second;
        varying.stops.clear();
        // Insert initial value
        size_t size = varyingTypeSize(varying.type);
        varying.stops.emplace_back(std::vector<char>(size));
    }
}

#define ADD_STOP_VALUE(__TYPE__) \
    const auto& stop = *reinterpret_cast<const __TYPE__*>(ptr); \
    auto& data = *reinterpret_cast<__TYPE__*>(dptr); \
    data += stop * a

void ShaderContext::prepareVaryings(const float* alpha, uint32_t count)
{
    count = std::min<size_t>(count, positions.size());
    for (auto& it : varyings)
    {
        const auto& name = it.first;
        DataType type = it.second.type;
        const auto& stops = it.second.stops;
        void* dptr = &it.second.data[0];

        // Clear data buffer
        size_t size = varyingTypeSize(type);
        memset(dptr, 0, size);

        for (uint32_t i = 0; i < count; ++i)
        {
            const void* ptr = &stops[i][0];
            float a = alpha[i];
            switch (type)
            {
            case VECTOR2:
            {
                ADD_STOP_VALUE(Vector2);
                break;
            }

            case VECTOR3:
            {
                ADD_STOP_VALUE(Vector3);
                break;
            }

            case VECTOR4:
            {
                ADD_STOP_VALUE(Vector4);
                break;
            }

            case COLOR3:
            {
                ADD_STOP_VALUE(Color3);
                break;
            }

            case COLOR4:
            {
                ADD_STOP_VALUE(Color4);
                break;
            }
            
            default:
                continue;

            }
        }
    }
}

void ShaderContext::reset()
{
    varyings.clear();
    _outPosition = Vector3();
    _outColor = Color4();
    positions.clear();
    num = 0;
    isFragment = false;
    isFirst = true;
}

}